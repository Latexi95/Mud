#include "maineventqueue.h"
#include <iostream>
#include "event.h"
#include "leveleventqueue.h"
#include <boost/atomic.hpp>

MainEventQueue *MEQ = 0;

MainEventQueue::MainEventQueue(int workerThreads) :
    mQueue(10),
    mWorkInQueue(0) {
    assert(MEQ == 0);
    MEQ = this;
    mWork = new boost::asio::io_service::work(mIoService);
    for (int i = 0; i < workerThreads; ++i) {
        mWorkers.emplace_back(new boost::thread([&]() {
            std::cout << "Started event queue thread " << boost::this_thread::get_id() << std::endl;
            mIoService.run();
            std::cout << "Stopped event queue thread " << boost::this_thread::get_id() << std::endl;
        }));
    }
}

MainEventQueue::~MainEventQueue()
{
    assert(MEQ);
    MEQ = 0;

}

MainEventQueue *MainEventQueue::instance()
{
    return MEQ;
}

void MainEventQueue::push(Event *e)
{
    if (!mQueue.push(e)) {
        std::cout << "MainEventQueue push failed" << std::endl;
    }
}

void MainEventQueue::push(time_type execTime, Event *e)
{
    mTimedEventQueue.push(execTime, e);
}

void MainEventQueue::handle(time_type t)
{
    mQueue.consume_all([this](Event *e) {
        mHandleQueue.push_back(e);
    });
    for (Event *e : mHandleQueue) {
        e->execute();
        if (e->eventLoopHasOwnership()) delete e;
    }
    mHandleQueue.clear();
    if (t != 0)
        mTimedEventQueue.advance(t);

    //Just to make sure there is not chance that event loop handling overlaps
    while (mWorkInQueue.load(boost::memory_order_acquire));

    for (const std::shared_ptr<LevelEventQueue> &levelEventQueue : mLevelEventQueues) {
        mWorkInQueue.fetch_add(1, boost::memory_order_relaxed);
        mIoService.post([this, levelEventQueue, t](){
            levelEventQueue->handle(t);

            mWorkInQueue.fetch_sub(1, boost::memory_order_release);
        });
    }

    while (mIoService.poll_one()) { }
}

void MainEventQueue::addLevelEventQueue(const std::shared_ptr<LevelEventQueue> &eq)
{
    mLevelEventQueues.push_back(eq);
}

void MainEventQueue::shutdown()
{
    mIoService.stop();
    delete mWork;
    mQueue.consume_all([](Event *e) {
        if (e->eventLoopHasOwnership()) delete e;
    });

    mIoService.stop();
    for (std::unique_ptr<boost::thread> &t : mWorkers) {
        t->join();
    }
}
