#include "timedeventqueue.h"
#include <iostream>
#include "events/event.h"

TimedEventQueue::TimedEventQueue()
{
}

TimedEventQueue::~TimedEventQueue() {
    while (!mQueue.empty()) {
        delete mQueue.top().second;
        mQueue.pop();
    }
}

void TimedEventQueue::push(time_type t, Event *e)
{
    boost::unique_lock<boost::mutex> lock(mMutex);
    mQueue.push(std::pair<time_type, Event*>(t, e));
}

void TimedEventQueue::advance(time_type currentTime)
{
    std::vector<std::pair<time_type, Event*>> handlingQueue;
    boost::unique_lock<boost::mutex> lock(mMutex);
    while (!mQueue.empty() && mQueue.top().first <= currentTime) {
        handlingQueue.push_back(mQueue.top());
        mQueue.pop();
    }
    lock.unlock();

    for (const auto &timedEvent : handlingQueue) {
        Event *e = timedEvent.second;
        e->execute();
        if (e->eventLoopHasOwnership()) delete e;
    }
}


