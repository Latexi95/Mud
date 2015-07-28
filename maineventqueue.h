#ifndef MAINEVENTQUEUE_H
#define MAINEVENTQUEUE_H
#include <boost/lockfree/queue.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>
#include <boost/atomic.hpp>
#include "common.h"
#include "timedeventqueue.h"
class LevelEventQueue;
class Event;


class MainEventQueue {
public:
    MainEventQueue(int workerThreads);
    ~MainEventQueue();

    static MainEventQueue *instance();

    void push(Event *e);
    void push(time_type execTime, Event *e);

    void handle(time_type t);

    void addLevelEventQueue(const std::shared_ptr<LevelEventQueue> &eq);

    void shutdown();
private:
    boost::asio::io_service mIoService;
    boost::asio::io_service::work *mWork;
    boost::lockfree::queue<Event*> mQueue;
    TimedEventQueue mTimedEventQueue;
    std::vector<Event*> mHandleQueue;
    std::vector<std::shared_ptr<LevelEventQueue> > mLevelEventQueues;
    std::vector<std::unique_ptr<boost::thread>> mWorkers;
    boost::atomic_int mWorkInQueue;
};

extern MainEventQueue *MEQ;

#endif // MAINEVENTQUEUE_H
