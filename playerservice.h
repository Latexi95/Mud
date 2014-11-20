#ifndef PLAYERSERVICE_H
#define PLAYERSERVICE_H
#include "player.h"
class Mud;
class PlayerService {
	public:
		PlayerService(Mud *mud);
		~PlayerService();
		static PlayerService *instance();

		RHandle<Player> findPlayerByName(const std::string &name);
		RHandle<Player> createPlayer(const std::string &name);
	private:
		Mud *mMud;
};

#endif // PLAYERSERVICE_H
