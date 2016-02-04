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

    bool loadResources();
    void shutdown();
    time_type time() const;
    time_type timeAfter(float seconds) const;

private:
    CommandService mCommandService;
    PlayerService mPlayerService;
    ResourceService mResourceService;
    LevelService mLevelService;
    CharacterService mCharacterService;
    MainEventQueue mEventQueue;
    boost::chrono::system_clock::time_point mLastUpdate;
    volatile bool mRunning;
    volatile time_type mTime;
};

extern Mud *MUD;

#endif // MUD_H
