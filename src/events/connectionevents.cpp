#include "connectionevents.h"
#include "character.h"
#include "client.h"
#include "level.h"
#include "playereventhandler.h"
#include "eventvisitor.h"

JoinEvent::JoinEvent(const std::shared_ptr<Client> &client, const std::shared_ptr<Character> &character) :
    mClient(client),
    mCharacter(character)
{

}

void JoinEvent::execute()
{
    mCharacter->room()->addCharacter(mCharacter);
    mCharacter->addEventHandler(std::unique_ptr<CharacterEventHandler>(new PlayerEventHandler(mClient)));
    mCharacter->room()->sendEventToCharacters(this);
}


DisconnectEvent::DisconnectEvent(const std::shared_ptr<Client> &client, const std::shared_ptr<Character> &character) :
    mClient(client),
    mCharacter(character)
{

}

void DisconnectEvent::execute()
{
    mCharacter->level()->sendEventToCharacters(this);
    mCharacter->room()->removeCharacter(mCharacter);
    mCharacter->removeEventHandlers();
}

IMPL_ACCEPT_EVENT_VISITOR(JoinEvent)
IMPL_ACCEPT_EVENT_VISITOR(DisconnectEvent)
