#include "gamemessagehandler.h"
#include "client.h"
GameMessageHandler::GameMessageHandler()
{
}

GameMessageHandler::~GameMessageHandler()
{

}

void GameMessageHandler::handle(Client *client, const std::string &message) {
	client->sendMessage("Game echo: " + message);
}
