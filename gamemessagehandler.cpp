#include "gamemessagehandler.h"
#include "client.h"
#include "level.h"
#include <boost/lexical_cast.hpp>
#include "leveleventqueue.h"
#include "event.h"
#include "playereventhandler.h"
#include "maineventqueue.h"
#include "messageevent.h"

GameMessageHandler::GameMessageHandler(Client *c, const std::shared_ptr<Character> &character):
	mCharacter(character),
	mPlayer(c->player()),
	mClient(c)
{
	std::shared_ptr<Level> level = mCharacter->level();
	level->eventQueue()->push(makeFunctionEvent([level, character](Event *e) {
		level->addCharacter(character);
	}));

	character->addEventHandler(std::unique_ptr<EventHandler>(new PlayerEventHandler(c)));
}

GameMessageHandler::~GameMessageHandler()
{

}

void GameMessageHandler::handle(Client *client, const std::string &message) {
	mCharacter->level()->eventQueue()->push(new MessageEvent(mCharacter, message));
}
