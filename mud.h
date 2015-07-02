#ifndef MUD_H
#define MUD_H
#include "gameeventloop.h"
#include "commandservice.h"
#include "playerservice.h"
#include "resourceservice.h"
#include "characterservice.h"
#include "levelservice.h"
#include "room.h"
#include <boost/thread.hpp>
class Mud {
	public:
		Mud();
		~Mud();

		CommandService &commandService() { return mCommandService; }
		void start();
		void stop();
	private:
		boost::thread *mEventLoopThread;
		GameEventLoop mEventLoop;
		CommandService mCommandService;
		PlayerService mPlayerService;
		ResourceService mResourceService;
		LevelService mLevelService;
		CharacterService mCharacterService;
};

#endif // MUD_H
