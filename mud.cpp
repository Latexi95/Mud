#include "mud.h"
#include "level.h"
//100ms
#define STEP_TIME 100
#define TIME_TICKS 1


namespace chrono = boost::chrono;
Mud *MUD = 0;

Mud::Mud() :
    mPlayerService(this),
    mEventQueue(2) {

    MUD = this;
    mLastUpdate = boost::chrono::system_clock::now();
    mTime = 0;
}

Mud::~Mud() {
}

void Mud::start() {
    mRunning = true;
    while (mRunning) {
        auto n = boost::chrono::system_clock::now();
        auto step = chrono::duration_cast<chrono::microseconds>(n - mLastUpdate);
        if (step.count() >= STEP_TIME) {
            mEventQueue.handle(mTime);
            mTime += TIME_TICKS;
            mLastUpdate += step;
        }
        else {
            mEventQueue.handle(0);
        }
    }

}

bool Mud::loadResources()
{
    if (!mResourceService.loadAllItemTemplates()) return false;
    if (!mLevelService.loadAllLevels()) return false;
    if (!mCharacterService.loadAllCharacters()) return false;

    return true;
}

void Mud::shutdown()
{
    std::cout << "Shutting down..." << std::endl;
    mRunning = false;
    mEventQueue.shutdown();
}

time_type Mud::time() const
{
    return mTime;
}

time_type Mud::timeAfter(float seconds) const
{
    return mTime + (time_type)(seconds * (1000.0 / STEP_TIME) * TIME_TICKS);
}

