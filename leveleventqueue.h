#ifndef LEVELEVENTQUEUE_H
#define LEVELEVENTQUEUE_H
#include <boost/lockfree/queue.hpp>
#include "common.h"
#include "timedeventqueue.h"

class Event;

class LevelEventQueue
{
	public:
		LevelEventQueue();
		~LevelEventQueue();
		void push(Event *e);
		void push(time_type execTime, Event *e);

		void handle(time_type t);
	private:
		boost::lockfree::queue<Event*> mQueue;
		TimedEventQueue mTimedEventQueue;
		std::vector<Event*> mHandleQueue;
};

#endif // LEVELEVENTQUEUE_H
