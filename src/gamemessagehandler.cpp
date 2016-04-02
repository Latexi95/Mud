#include "gamemessagehandler.h"
#include "client.h"
#include "level.h"
#include <boost/lexical_cast.hpp>
#include "leveleventqueue.h"
#include "events/event.h"
#include "playereventhandler.h"
#include "maineventqueue.h"
#include "events/messageevent.h"
#include "levelservice.h"
#include <boost/algorithm/string.hpp>
#include "events/connectionevents.h"
#include "commandservice.h"


GameMessageHandler::GameMessageHandler(Client *c, const std::shared_ptr<Character> &character):
    mPlayer(c->player()),
    mClient(c),
    mCommandParserSelection(CommandParserSelection::Default)
{
    c->setCharacter(character);
    Level *level = LS->level(character->levelId());
    character->setRoom(level->roomById(character->roomId()));
    level->eventQueue()->push(new JoinEvent(c->shared_from_this(), character));
}

GameMessageHandler::~GameMessageHandler()
{

}

void GameMessageHandler::handleCommand(const std::shared_ptr<Client> &client, const std::string &message)
{
    CommandContext ctx;
    ctx.mCaller = character();
    const CommandParser &parser = CMDS->standardCommandParser();
    parser.parse(message, std::move(ctx), client);
}

void GameMessageHandler::handleEditorCommand(const std::shared_ptr<Client> &client, const std::string &message)
{
    CommandContext ctx;
    ctx.mCaller = character();
    const CommandParser &parser = CMDS->editorCommandParser();
    parser.parse(message, std::move(ctx), client);
}

const std::shared_ptr<Character> &GameMessageHandler::character() const
{
    return mClient->character();
}

LevelEventQueue *GameMessageHandler::levelQueue() const
{
    return character()->level()->eventQueue();
}

void GameMessageHandler::handle(const std::shared_ptr<Client> &client, const std::string &message) {
    if (message == "$") {
        switch (mCommandParserSelection) {
        case CommandParserSelection::Default:
            mCommandParserSelection = CommandParserSelection::Editor;
            client->ui().send("Switched to edit mode");
            break;
        case CommandParserSelection::Editor:
            mCommandParserSelection = CommandParserSelection::Default;
            client->ui().send("Switched to default mode");
            break;
        }
        return;
    }
    if (message.size() > 1 && message[0] == '!') {
        levelQueue()->push(new MessageEvent(client->character(), message.substr(1)));
    }

    if (mCommandParserSelection == CommandParserSelection::Default) {
        if (message.size() >= 1 && message[0] == '$') {
            handleEditorCommand(client, message.substr(1));
            return;
        }
        else if (message.size() >= 1 && message[0] != '!' && message[0] != ' ') {
            handleCommand(client, message);
            return;
        }

    }
    else {
        handleEditorCommand(client, message);
        return;
    }

    levelQueue()->push(new MessageEvent(client->character(), message));
}

void GameMessageHandler::disconnected(const std::shared_ptr<Client> &client)
{
    levelQueue()->push(new DisconnectEvent(client, character()));
}
