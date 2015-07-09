#include "playereventhandler.h"
#include "client.h"
#include "messageevent.h"

PlayerEventHandler::PlayerEventHandler(Client *c) :
    mClient(c)
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
    default:
        break;
    }
}
