#include "joinmessagehandler.h"
#include "client.h"
#include <boost/regex.h>
#include <boost/algorithm/string/trim_all.hpp>
#include <boost/make_shared.hpp>
#include "gamemessagehandler.h"
#include "resourceservice.h"
#include "playerservice.h"
JoinMessageHandler::JoinMessageHandler() :
	mState(ExpectingName){
}

JoinMessageHandler::~JoinMessageHandler() {

}

void JoinMessageHandler::handle(Client *client, const std::string &message) {
	if (message.empty()) return;
	std::string trimmed = boost::algorithm::trim_all_copy(message);
	switch (mState) {
		case ExpectingName: {
			if (trimmed.empty()) {
				client->sendMessage("What's our name survivor?" + std::to_string((int)message.at(0)));
				return;
			}
			if (trimmed.size() < 2 || message.size() > 20) {
				client->sendMessage("Uh... I think that isn't our name."
									"(A character name should be between 2 and 20 characters.)");
				return;
			}
			RHandle<Player> player = PlayerService::instance()->findPlayerByName(trimmed);
			if (player.isNull()) {
				player = PlayerService::PlayerService::instance()->createPlayer(trimmed);
				client->sendMessage("Can't find a character named \"" + trimmed + "\"");
				client->sendMessage("Creating a new character with given name.");
				clinet->sendMessage("Input a password");
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
				if (player->isLoggedIn()) {
					client->sendMessage("Someone has already logged in.");
					client->sendMessage("Disconnecting");
					break;
				}
				client->sendMessage("Successfully logged in");
				clinet->player()->setLoggedIn(true);
				client->setMessageHandler(boost::make_shared<GameMessageHandler>());
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
				ResourceService::instance()->save(client->player());
				client->setMessageHandler(boost::make_shared<GameMessageHandler>());
			}
			else {
				client->sendMessage("Password didn't match the password you first gave. Try again.");
				mState = ExpectingNewPassword;
			}
			break;
	}


}
