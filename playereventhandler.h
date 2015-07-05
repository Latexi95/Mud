#ifndef PLAYEREVENTHANDLER_H
#define PLAYEREVENTHANDLER_H
#include "eventhandler.h"
class Client;
class PlayerEventHandler : public EventHandler
{
	public:
		PlayerEventHandler(Client *c);
		~PlayerEventHandler();

		void handleEvent(Event *e);

	private:
		Client *mClient;
};

#endif // PLAYEREVENTHANDLER_H
