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
    level->eventQueue()->push(new JoinEvent(c->shared_from_this(), mCharacter));
}

GameMessageHandler::~GameMessageHandler()
{

}

void GameMessageHandler::handle(const std::shared_ptr<Client> &client, const std::string &message) {
    if (message.size() >= 1 && message[0] != '!') {
        std::vector<std::string> params;
        Command *cmd = mCommandParser.parse(message, params);
        if (!cmd) {
            std::vector<std::string> errorMsgParts;
            boost::split(errorMsgParts, mCommandParser.errorMessage(), boost::is_any_of("\n"));
            for (const std::string &msg : errorMsgParts) {
                client->sendMessage(msg);
            }

        }
    }
    else {
        if (message.size() > 1 && message[0] == '!') {
            mCharacter->level()->eventQueue()->push(new MessageEvent(mCharacter, message.substr(1)));
        }

    }
}

void GameMessageHandler::disconnected(const std::shared_ptr<Client> &client)
{
    Level *level = mCharacter->level();
    level->eventQueue()->push(new DisconnectEvent(client, mCharacter));
}
