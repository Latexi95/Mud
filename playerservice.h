#ifndef PLAYERSERVICE_H
#define PLAYERSERVICE_H
#include "player.h"
class PlayerService {
	public:
		PlayerService();
		Player *findPlayerByName(const std::string &name);
	private:

};

#endif // PLAYERSERVICE_H
