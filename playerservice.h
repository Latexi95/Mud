#ifndef PLAYERSERVICE_H
#define PLAYERSERVICE_H
#include "player.h"
#include <memory>
#include <boost/thread/mutex.hpp>
class Mud;
class PlayerService {
	public:
		PlayerService(Mud *mud);
		~PlayerService();
		static PlayerService *instance();

		std::shared_ptr<Player> findPlayerByName(const std::string &name);
		std::shared_ptr<Player> createPlayer(const std::string &name);
		void savePlayers();
	private:
		Mud *mMud;
		boost::mutex mMutex;
		std::vector<std::shared_ptr<Player> > mPlayers;
};

#endif // PLAYERSERVICE_H
