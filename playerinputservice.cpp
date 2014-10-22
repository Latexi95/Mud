#include "playerinputservice.h"
#include "event.h"
#include "commandevent.h"
#include <iostream>
#include "gameeventloop.h"
#include "character.h"
PlayerInputService::PlayerInputService() :
	mMessageQueue(100),
	mPlayer(0)
{
}

void PlayerInputService::handleEvent(Event *e) {
	switch (e->type()) {
		case Event::Command:
			mMessageQueue.push(new std::string(static_cast<CommandEvent*>(e)->command()));
			break;
	}
}

std::vector<int> PlayerInputService::handledEventTypes() const {
	std::vector<int> handledEventTypes;
	handledEventTypes.push_back(Event::Command);
	return handledEventTypes;
}

void PlayerInputService::loop() {
	std::cout << "Input service started: \n";
	std::string cmd;
	while(std::getline(std::cin, cmd)) {
		if (cmd == "exit") break;

		CommandEvent *ce = new CommandEvent(cmd, mPlayer);
		GameEventLoop::instance()->push(ce);
		std::string *msg;
		while (mMessageQueue.pop(msg)) {
			std::cout << *msg << std::endl;
			delete msg;
		}

	}
}

void PlayerInputService::setPlayer(Character *player) {
	mPlayer = player;
}
