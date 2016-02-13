#include "playereventhandler.h"
#include "client.h"
#include "events/messageevent.h"
#include "messagebuilder.h"
#include "events/connectionevents.h"
#include "events/moveevent.h"
#include "room.h"

PlayerEventHandler::PlayerEventHandler(const std::shared_ptr<Client> &client) :
    mClient(client)
{
}

PlayerEventHandler::~PlayerEventHandler()
{
}

void PlayerEventHandler::handleEvent(const std::shared_ptr<Character> &character, Event *e)
{
    mCharacter = character;
    e->accept(this);
}

void PlayerEventHandler::visit(MessageEvent *msgEvent)
{
    if (msgEvent->sender() != mCharacter) {
        mClient->sendMessage(MessageBuilder() << msgEvent->sender() << ": " << msgEvent->message());
    }
}

void PlayerEventHandler::visit(JoinEvent *e)
{
    mClient->sendMessage(MessageBuilder("Player ") << e->character() << " joined the game.");
}

void PlayerEventHandler::visit(DisconnectEvent *e)
{
    mClient->sendMessage(MessageBuilder("Player ") << e->character() << " disconnected from the game.");
}

void PlayerEventHandler::visit(MoveStartEvent *e)
{
    if (e->character() == mCharacter) {
        mClient->sendMessage(MessageBuilder() << "Started moving");
    }
}

void PlayerEventHandler::visit(MoveEndEvent *e)
{
    Room *r = e->targetRoom();
    if (e->character() == mCharacter) {
        mClient->sendMessage(MessageBuilder() << "Arrived " << r->name());
        mClient->sendMessage(MessageBuilder() << "Target");
    }
    else {
        mClient->sendMessage(MessageBuilder() << e->character() << " walked here");
    }
}
