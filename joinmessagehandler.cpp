#include "joinmessagehandler.h"
#include "client.h"
#include <boost/regex.h>
#include <boost/algorithm/string/trim_all.hpp>
#include <boost/make_shared.hpp>
#include "gamemessagehandler.h"
#include "charactercreationmessagehandler.h"
#include "resourceservice.h"
#include "playerservice.h"
#include <boost/algorithm/string/join.hpp>
#include "textgen/textutils.h"
#include <algorithm>

JoinMessageHandler::JoinMessageHandler() :
	mState(ExpectingName){
}

JoinMessageHandler::~JoinMessageHandler() {

}

void JoinMessageHandler::gotoCharacterCreation(Client *client) {
	client->sendMessage("Going to character creation ->");
	std::shared_ptr<CharacterCreationMessageHandler> charCreation = std::make_shared<CharacterCreationMessageHandler>();
	charCreation->sendCharacterCreationInitMessage(client);
	client->setMessageHandler(charCreation);
}

void JoinMessageHandler::handle(Client *client, const std::string &message) {
	if (message.empty()) return;
	std::string trimmed = boost::algorithm::trim_all_copy(message);
	switch (mState) {
		case ExpectingName: {
			if (trimmed.empty()) {
				client->sendMessage("What's your account name?");
				return;
			}
			if (trimmed.size() < 2 || message.size() > 20) {
				client->sendMessage("Uh... I don't think that's the correct one. "
									"(A player name should be between 2 and 20 characters.)");
				return;
			}
			std::shared_ptr<Player> player = PlayerService::instance()->findPlayerByName(trimmed);
			if (!player) {
				player = PlayerService::PlayerService::instance()->createPlayer(trimmed);
				client->sendMessage("Can't find a character named \"" + trimmed + "\"");
				client->sendMessage("Creating a new character with given name.");
				client->sendMessage("Input a password");
				mState = ExpectingNewPassword;
				player->setName(trimmed);
			}
			else {
				mState = ExpectingPassword;
				client->sendMessage("Input the password");
			}
			client->setPlayer(player);
			break;
		}
		case ExpectingPassword: {
			if (client->player()->passwordMatch(trimmed)) {
				if (client->player()->isLoggedIn()) {
					client->sendMessage("Someone has already logged in.");
					client->sendMessage("Disconnecting");
					client->disconnect();
					break;
				}
				client->sendMessage("Successfully logged in");
				client->player()->setLoggedIn(true);

				if (client->player()->characterNames().empty()) {
					client->sendMessage("You have no characters.");
					gotoCharacterCreation(client);
					return;
				} else {
					client->sendMessage("You have characters: "
										+ boost::algorithm::join(client->player()->characterNames(), ", "));
					client->sendMessage("Write the name of the character you wish to play or write \"create\" to create a new character");
					mState = CharacterSelection;
				}
				break;
			}
			else {
				client->sendMessage("Wrong password. Try again.");
			}
			break;
		}
		case ExpectingNewPassword: {
			client->player()->setPassword(trimmed);
			client->sendMessage("Write the password again");
			mState = ExpectingPasswordConfirmation;
			break;
		}
		case ExpectingPasswordConfirmation:
			if (client->player()->passwordMatch(trimmed)) {
				client->sendMessage("Successfully created a character and logged in");
				client->player()->setLoggedIn(true);
				PlayerService::instance()->savePlayers();

				gotoCharacterCreation(client);
				return;
			}
			else {
				client->sendMessage("Password didn't match the password you first gave. Try again.");
				mState = ExpectingNewPassword;
			}
			break;
		case CharacterSelection: {
			std::string characterName = text::cleanFolded(message);
			if (characterName == "create") {
				gotoCharacterCreation(client);
				return;
			} else {
				const std::vector<std::string> cn = client->player()->characterNames();
				for (const std::string &charName : cn) {
					if (text::cleanFolded(charName) == characterName) {
						client->sendMessage("Starting an adventure");
						client->setMessageHandler(std::make_shared<GameMessageHandler>());
						return;
					}
				}
				client->sendMessage("Unknown character name. Try again.");
				return;
			}

		}
	}


}
