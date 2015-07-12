#include "connectionevents.h"
#include "character.h"
#include "client.h"
#include "level.h"
#include "playereventhandler.h"

JoinEvent::JoinEvent(const std::shared_ptr<Client> &client, const std::shared_ptr<Character> &character) :
    mClient(client),
    mCharacter(character)
{

}

void JoinEvent::execute()
{
    mCharacter->level()->addCharacter(mCharacter);
    mCharacter->addEventHandler(std::unique_ptr<CharacterEventHandler>(new PlayerEventHandler(mClient)));

    mCharacter->level()->sendEventToCharacters(this);
}


DisconnectEvent::DisconnectEvent(const std::shared_ptr<Client> &client, const std::shared_ptr<Character> &character) :
    mClient(client),
    mCharacter(character)
{

}

void DisconnectEvent::execute()
{
    mCharacter->level()->sendEventToCharacters(this);
    mCharacter->level()->removeCharacter(mCharacter);
    mCharacter->removeEventHandlers();
}
