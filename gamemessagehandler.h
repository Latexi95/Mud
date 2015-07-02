#ifndef GAMEMESSAGEHANDLER_H
#define GAMEMESSAGEHANDLER_H
#include "messagehandler.h"
#include <memory>
class Player;
class Character;

class GameMessageHandler : public MessageHandler {
	public:
		GameMessageHandler(Client *c, const std::shared_ptr<Character> &character);
		~GameMessageHandler();
		void handle(Client *client, const std::string &message);
	private:
		std::shared_ptr<Character> mCharacter;
		std::shared_ptr<Player> mPlayer;
		Client *mClient;

};

#endif // GAMEMESSAGEHANDLER_H
