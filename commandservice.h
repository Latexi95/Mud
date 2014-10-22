#ifndef COMMANDSERVICE_H
#define COMMANDSERVICE_H
#include "eventhandler.h"
class CommandEvent;
class CommandService : public EventHandler {
	public:
		CommandService();

		void handleEvent(Event *e);
		void handleEvent(CommandEvent *e);
		std::vector<int> handledEventTypes() const;
};

#endif // COMMANDSERVICE_H
