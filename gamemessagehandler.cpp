#include "gamemessagehandler.h"
#include "client.h"
GameMessageHandler::GameMessageHandler(Client *c, const std::shared_ptr<Character> &character):
	mClient(c),
	mCharacter(character),
	mPlayer(c->player())
{

}

GameMessageHandler::~GameMessageHandler()
{

}

void GameMessageHandler::handle(Client *client, const std::string &message) {
	client->sendMessage("Game echo: " + message);
}
