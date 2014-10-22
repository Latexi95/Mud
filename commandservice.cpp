#include "commandservice.h"
#include "event.h"
#include "commandevent.h"
#include <iostream>

CommandService::CommandService()
{
}

void CommandService::handleEvent(Event *e) {
	switch (e->type()) {
	case Event::Command:
		handleEvent(static_cast<CommandEvent*>(e));
		break;
	}
}

void CommandService::handleEvent(CommandEvent *e) {
	std::cout << e->command();
}

std::vector<int> CommandService::handledEventTypes() const {
	std::vector<int> r;
	r.push_back(Event::Command);
	return r;
}
