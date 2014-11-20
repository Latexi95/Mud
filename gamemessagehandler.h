#ifndef GAMEMESSAGEHANDLER_H
#define GAMEMESSAGEHANDLER_H
#include "messagehandler.h"

class GameMessageHandler : public MessageHandler {
	public:
		GameMessageHandler();
		~GameMessageHandler();
		void handle(Client *client, const std::string &message);

};

#endif // GAMEMESSAGEHANDLER_H
