#include "playereventhandler.h"
#include "client.h"
#include "messageevent.h"
#include "messagebuilder.h"
#include "connectionevents.h"

PlayerEventHandler::PlayerEventHandler(const std::shared_ptr<Client> &client) :
    mClient(client)
{
}

void PlayerEventHandler::handleEvent(Event *e)
{
    switch (e->type()) {
    case Event::Message: {
        MessageEvent *msgEvent = static_cast<MessageEvent*>(e);
        mClient->sendMessage("<" + msgEvent->sender()->name() + ">: " + msgEvent->message());
        break;
    }
    case Event::Join: {
        JoinEvent *joinEvent = static_cast<JoinEvent*>(e);
        mClient->sendMessage(MessageBuilder("Player ") << joinEvent->character() << " joined the game.");
        break;
    }
    case Event::Disconnect: {
        DisconnectEvent *disconnectEvent = static_cast<DisconnectEvent*>(e);
        mClient->sendMessage(MessageBuilder("Player ") << disconnectEvent->character() << " disconnected from the game.");
        break;
    }
    default:
        break;
    }
}
