#include "charactercreationmessagehandler.h"
#include "client.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

CharacterCreationMessageHandler::CharacterCreationMessageHandler() :
	mStatus(InitMessage),
	mGender(Character::Male),
	mAge(25),
	mHairColor(0){


}

CharacterCreationMessageHandler::~CharacterCreationMessageHandler() {

}

void CharacterCreationMessageHandler::sendCharacterCreationInitMessage(Client *c) {
	c->sendMessage("");
	c->sendMessage("Character creation");
	c->sendMessage("------------------");
	mStatus = GenderSelection;
	mStatusStack.push(NameSelection);
	mStatusStack.push(AgeSelection);
	mStatusStack.push(HairColorSelection);
	mStatusStack.push(BackgroundSelection);
}

void CharacterCreationMessageHandler::handle(Client *c, const std::string &message) {

	switch (mStatus) {
		case InitMessage:
			sendCharacterCreationInitMessage(c);
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
				c->sendMessage("Choose your character's gender: m / f");
				break;
			}
			mStatus = mStatusStack.front(); mStatusStack.pop();
			break;
		}
		case NameSelection: {
			std::string name = boost::algorithm::trim_copy(message);
			if (name.empty()) {
				c->sendMessage("What is the name of your character?");
				return;
			}

			c->sendMessage("Your character name is \"" + message + "\"");

			mStatus = mStatusStack.front(); mStatusStack.pop();
			break;
		}
		case AgeSelection: {
			std::string ageText = boost::algorithm::trim_copy(message);
			try {
				int num = boost::lexical_cast<int>(ageText);
				if (num < 16 || num > 70) {
					c->sendMessage("Give a number between 16 and 70");
					return;
				}
				mAge = num;
			} catch (const boost::bad_lexical_cast &e) {
				c->sendMessage("Give a number for the character's age");
				return;
			}

			mStatus = mStatusStack.front(); mStatusStack.pop();
			break;
		}
		case HairColorSelection: {
			text::Color *color = text::colorByName(message);
			if (!color) {
				c->sendMessage("Unknown color \"" + message + "\". Try again:");
				return;
			}
			mHairColor = color;
			mStatus = mStatusStack.front(); mStatusStack.pop();
			break;
		}
		case BackgroundSelection: {
			c->sendMessage("...");
			return;
		}
	}

	switch (mStatus) {
		case InitMessage:
			break;
		case GenderSelection:
			c->sendMessage("Select gender: male(m), female(f)");
			break;
		case NameSelection:
			c->sendMessage("Character name:");
			break;
		case AgeSelection:
			c->sendMessage("Character age:");
			break;
		case HairColorSelection:
			c->sendMessage("Hair color:");
			break;
		case BackgroundSelection:
			c->sendMessage("Background stuff");
	}

}
