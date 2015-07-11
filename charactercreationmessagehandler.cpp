#include "charactercreationmessagehandler.h"
#include "client.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "levelservice.h"
#include "characterservice.h"
#include "gamemessagehandler.h"
#include "textgen/textutils.h"
#include "player.h"
#include "playerservice.h"

CharacterCreationMessageHandler::CharacterCreationMessageHandler() :
    mStatus(InitMessage),
    mGender(Character::Male),
    mAge(25),
    mHairColor(0){


}

CharacterCreationMessageHandler::~CharacterCreationMessageHandler() {

}

void CharacterCreationMessageHandler::sendCharacterCreationInitMessage(const std::shared_ptr<Client> &client) {
    client->sendMessage("");
    client->sendMessage("Character creation");
    client->sendMessage("------------------");
    client->sendMessage("Select gender: male(m), female(f)");
    mStatus = GenderSelection;
    mStatusStack.push(NameSelection);
    mStatusStack.push(AgeSelection);
    mStatusStack.push(HairColorSelection);
    mStatusStack.push(InsertToMap);
}

void CharacterCreationMessageHandler::handle(const std::shared_ptr<Client> &client, const std::string &message) {

    switch (mStatus) {
    case InitMessage:
        sendCharacterCreationInitMessage(client);
        break;
    case GenderSelection: {
        std::string name = boost::algorithm::trim_copy(message);
        if (name == "male" || name == "m") {
            mGender = Character::Male;
        }
        else if (name == "female" || name == "f"){
            mGender = Character::Female;
        }
        else {
            client->sendMessage("Choose your character's gender: m / f");
            break;
        }
        mStatus = mStatusStack.front(); mStatusStack.pop();
        break;
    }
    case NameSelection: {
        std::string name = text::cleanFolded(message);
        if (name.empty()) {
            client->sendMessage("What is the name of your character?");
            return;
        }

        client->sendMessage("Your character name is \"" + message + "\"");

        mName = message;


        mStatus = mStatusStack.front(); mStatusStack.pop();
        break;
    }
    case AgeSelection: {
        std::string ageText = boost::algorithm::trim_copy(message);
        try {
            int num = boost::lexical_cast<int>(ageText);
            if (num < 16 || num > 70) {
                client->sendMessage("Give a number between 16 and 70");
                return;
            }
            mAge = num;
        } catch (const boost::bad_lexical_cast &e) {
            client->sendMessage("Give a number for the character's age");
            return;
        }

        mStatus = mStatusStack.front(); mStatusStack.pop();
        break;
    }
    case HairColorSelection: {
        text::Color *color = text::colorByName(message);
        if (!color) {
            client->sendMessage("Unknown color \"" + message + "\". Try again:");
            return;
        }
        mHairColor = color;
        mStatus = mStatusStack.front(); mStatusStack.pop();
        break;
    }
    case InsertToMap: {
        return;
    }
    }

    switch (mStatus) {
    case InitMessage:
        break;
    case GenderSelection:
        client->sendMessage("Select gender: male(m), female(f)");
        break;
    case NameSelection:
        client->sendMessage("Character name:");
        break;
    case AgeSelection:
        client->sendMessage("Character age:");
        break;
    case HairColorSelection:
        client->sendMessage("Hair color:");
        break;
    case InsertToMap: {
        client->sendMessage("Inserting the character to the world");

        std::shared_ptr<Level> beach = LS->level("beach");
        std::shared_ptr<Character> character = CS->createCharacter(mName);
        character->setGender(mGender);
        character->setAge(mAge);
        character->setLevel(beach);
        character->setPos(Position( 2, 2));

        CS->saveCharacter(character);

        client->player()->addCharacterName(mName);

        PlayerService::instance()->savePlayers();

        client->setMessageHandler(std::make_shared<GameMessageHandler>(client.get(), character));
        break;
    }
    }

}

void CharacterCreationMessageHandler::disconnected(const std::shared_ptr<Client> &client)
{

}
