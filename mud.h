#ifndef MUD_H
#define MUD_H
#include "maineventqueue.h"
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

    MainEventQueue &mainEventQueue() { return mEventQueue; }
private:
    CommandService mCommandService;
    PlayerService mPlayerService;
    ResourceService mResourceService;
    LevelService mLevelService;
    CharacterService mCharacterService;
    MainEventQueue mEventQueue;
    boost::chrono::system_clock::time_point mLastUpdate;
    volatile bool mRunning;
    time_type mTime;
};

#endif // MUD_H
