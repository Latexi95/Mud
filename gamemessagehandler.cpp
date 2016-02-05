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

void GameMessageHandler::sendCommandErrorMessage(const std::shared_ptr<Client> &client)
{
    std::vector<std::string> errorMsgParts;
    boost::split(errorMsgParts, mCommandParser.errorMessage(), boost::is_any_of("\n"));
    for (const std::string &msg : errorMsgParts) {
        client->sendMessage(msg);
    }
}

void GameMessageHandler::handleCommand(const std::string &message, const std::shared_ptr<Client> &client)
{
    std::vector<std::string> params;
    Command *cmd = mCommandParser.parse(message, params);
    if (!cmd) {
        sendCommandErrorMessage(client);
    }
    else {
        CommandContext ctx;
        ctx.mCaller = mCharacter;
        ctx.mParameters = std::move(params);
        cmd->execute(ctx);
    }
}

LevelEventQueue *GameMessageHandler::levelQueue() const
{
    mCharacter->level()->eventQueue();
}

void GameMessageHandler::handle(const std::shared_ptr<Client> &client, const std::string &message) {
    if (message.size() >= 1 && message[0] != '!') {
        handleCommand(message, client);
    }
    else {
        if (message.size() > 1 && message[0] == '!') {
            mCharacter->level()->eventQueue()->push(new MessageEvent(mCharacter, message.substr(1)));
        }
        else {
            mCharacter->level()->eventQueue()->push(new MessageEvent(mCharacter, message.substr(1)));
        }
    }
}

void GameMessageHandler::disconnected(const std::shared_ptr<Client> &client)
{
    Level *level = mCharacter->level();
    level->eventQueue()->push(new DisconnectEvent(client, mCharacter));
}
