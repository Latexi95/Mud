#ifndef PLAYERINPUTSERVICE_H
#define PLAYERINPUTSERVICE_H
#include "eventhandler.h"
#include <boost/lockfree/queue.hpp>

class Character;
class PlayerInputService : public EventHandler {
	public:
		PlayerInputService();
		void handleEvent(Event *e);
		std::vector<int> handledEventTypes() const;
		void loop();
		void setPlayer(Character *player);
	private:
		Character *mPlayer;
		boost::lockfree::queue<std::string*> mMessageQueue;
};

#endif // PLAYERINPUTSERVICE_H
