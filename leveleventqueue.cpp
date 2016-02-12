#include "leveleventqueue.h"
#include "events/event.h"
#include "level.h"

LevelEventQueue::LevelEventQueue(Level *l) :
    mQueue(10),
    mLevel(l)
{

}

LevelEventQueue::~LevelEventQueue()
{
    mQueue.consume_all([](Event *e) {
        if (e->eventLoopHasOwnership()) delete e;
    });
}

void LevelEventQueue::push(Event *e)
{
    if (!mQueue.push(e)) {
        std::cout << "LevelEventQueue push failed" << std::endl;
    }
}

void LevelEventQueue::push(time_type execTime, Event *e)
{
    mTimedEventQueue.push(execTime, e);
}

void LevelEventQueue::handle(time_type t) {
    mQueue.consume_all([this](Event *e) {
        mHandleQueue.push_back(e);
    });

    LEVEL = mLevel;
    for (Event *e : mHandleQueue) {
        e->execute();
        if (e->eventLoopHasOwnership()) delete e;
    }
    mHandleQueue.clear();

    if (t != 0) {
        mTimedEventQueue.advance(t);
    }

    LEVEL = 0;
}
