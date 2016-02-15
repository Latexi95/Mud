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
    mCharacter(character),
    mPlayer(c->player()),
    mClient(c)
{
    Level *level = LS->level(mCharacter->levelId());
    mCharacter->setRoom(level->roomById(mCharacter->roomId()));
    level->eventQueue()->push(new JoinEvent(c->shared_from_this(), mCharacter));
}

GameMessageHandler::~GameMessageHandler()
{

}

void GameMessageHandler::handleCommand(const std::shared_ptr<Client> &client, const std::string &message)
{
    CommandContext ctx;
    ctx.mCaller = mCharacter;
    const CommandParser &parser = CMDS->standardCommandParser();
    parser.parse(message, std::move(ctx), client);
}

void GameMessageHandler::handleEditorCommand(const std::shared_ptr<Client> &client, const std::string &message)
{
    CommandContext ctx;
    ctx.mCaller = mCharacter;
    const CommandParser &parser = CMDS->editorCommandParser();
    parser.parse(message, std::move(ctx), client);
}

LevelEventQueue *GameMessageHandler::levelQueue() const
{
    return mCharacter->level()->eventQueue();
}

void GameMessageHandler::handle(const std::shared_ptr<Client> &client, const std::string &message) {
    if (message.size() >= 1 && message[0] == '$') {
        handleEditorCommand(client, message.substr(1));
    }
    else if (message.size() >= 1 && message[0] != '!' && message[0] != ' ') {
        handleCommand(client, message);
    }
    else {
        if (message.size() > 1 && message[0] == '!') {
            mCharacter->level()->eventQueue()->push(new MessageEvent(mCharacter, message.substr(1)));
        }
        else {
            mCharacter->level()->eventQueue()->push(new MessageEvent(mCharacter, message));
        }
    }
}

void GameMessageHandler::disconnected(const std::shared_ptr<Client> &client)
{
    Level *level = mCharacter->level();
    level->eventQueue()->push(new DisconnectEvent(client, mCharacter));
}
