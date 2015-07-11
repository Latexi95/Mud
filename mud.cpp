#include "mud.h"
#include "level.h"
#define STEP_TIME 0.1
#define TIME_TICKS 1
Mud::Mud() :
    mPlayerService(this),
    mEventQueue(2) {

    mLastUpdate = boost::chrono::system_clock::now();
    mTime = 0;
}

Mud::~Mud() {
}

void Mud::start() {
    mRunning = true;
    while (mRunning) {
        auto n = boost::chrono::system_clock::now();
        auto step = boost::chrono::system_clock::time_point::duration(n - mLastUpdate);
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

void Mud::shutdown()
{
    std::cout << "Shutting down..." << std::endl;
    mRunning = false;
    mEventQueue.shutdown();
}

