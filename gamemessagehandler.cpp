#include "gamemessagehandler.h"
#include "client.h"
#include "level.h"
#include <boost/lexical_cast.hpp>
#include "leveleventqueue.h"
#include "events/event.h"
#include "playereventhandler.h"
#include "maineventqueue.h"
#include "events/messageevent.h"

#include <boost/algorithm/string.hpp>
#include "events/connectionevents.h"



GameMessageHandler::GameMessageHandler(Client *c, const std::shared_ptr<Character> &character):
    mCharacter(character),
    mPlayer(c->player()),
    mClient(c)
{
    std::shared_ptr<Level> level = mCharacter->level();
    level->eventQueue()->push(new JoinEvent(c->shared_from_this(), mCharacter));
}

GameMessageHandler::~GameMessageHandler()
{

}

void GameMessageHandler::handle(const std::shared_ptr<Client> &client, const std::string &message) {
    if (message.size() > 2 && message[0] == '!' && message[1] != '!') {
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
        mCharacter->level()->eventQueue()->push(new MessageEvent(mCharacter, message));
    }
}

void GameMessageHandler::disconnected(const std::shared_ptr<Client> &client)
{
    std::shared_ptr<Level> level = mCharacter->level();
    level->eventQueue()->push(new DisconnectEvent(client, mCharacter));
}
