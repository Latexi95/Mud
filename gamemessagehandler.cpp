#include "gamemessagehandler.h"
#include "client.h"
#include "level.h"
#include <boost/lexical_cast.hpp>

GameMessageHandler::GameMessageHandler(Client *c, const std::shared_ptr<Character> &character):
	mCharacter(character),
	mPlayer(c->player()),
	mClient(c)
{
	std::shared_ptr<Level> level = mCharacter->level();
	level->addCharacter(character);
}

GameMessageHandler::~GameMessageHandler()
{

}

void GameMessageHandler::handle(Client *client, const std::string &message) {
	std::shared_ptr<Level> level = mCharacter->level();

	Room room = level->room(mCharacter->pos().x(), mCharacter->pos().y());
	if (message == "look") {
		for (const std::pair<std::string, std::string> &lp : room.lookMap()) {
			client->sendMessage("Looks: " + lp.first + ": " + lp.second);
		}
	}
	else if (message == "n") {
		Position newPos = mCharacter->pos();
		newPos.mY -= 1;
		level->moveCharacter(mCharacter, newPos);
		client->sendMessage("Moved to " + boost::lexical_cast<std::string>(newPos.x()) + ", " + boost::lexical_cast<std::string>(newPos.y()));
	}

}
