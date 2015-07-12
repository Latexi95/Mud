#include "playereventhandler.h"
#include "client.h"
#include "events/messageevent.h"
#include "messagebuilder.h"
#include "events/connectionevents.h"

PlayerEventHandler::PlayerEventHandler(const std::shared_ptr<Client> &client) :
    mClient(client)
{
}

void PlayerEventHandler::handleEvent(const std::shared_ptr<Character> &character, Event *e)
{
    switch (e->type()) {
    case Event::Message: {
        MessageEvent *msgEvent = static_cast<MessageEvent*>(e);
        if (msgEvent->sender() != character) {
            mClient->sendMessage(MessageBuilder() << msgEvent->sender() << MessageBuilder::NoSpace << ": " << msgEvent->message());
        }
        break;
    }
    case Event::Join: {
        JoinEvent *joinEvent = static_cast<JoinEvent*>(e);
        mClient->sendMessage(MessageBuilder("Player ") << joinEvent->character() << "joined the game.");
        break;
    }
    case Event::Disconnect: {
        DisconnectEvent *disconnectEvent = static_cast<DisconnectEvent*>(e);
        mClient->sendMessage(MessageBuilder("Player ") << disconnectEvent->character() << "disconnected from the game.");
        break;
    }
    default:
        break;
    }
}
