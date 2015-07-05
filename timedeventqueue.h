#ifndef TIMEDEVENTQUEUE_H
#define TIMEDEVENTQUEUE_H
#include <queue>
#include <boost/thread/mutex.hpp>
#include <cstdint>
#include "common.h"
class Event;

class TimedEventQueue {
	public:

		TimedEventQueue();
		~TimedEventQueue();
		void push(time_type t, Event *e);

		void advance(time_type currentTime);
	private:
		struct PriorityQueueComparator {
				bool operator()(const std::pair<time_type, Event*> &a, const std::pair<time_type, Event*> &b) {
					return a.first > b.first;
				}
		};


		std::priority_queue<std::pair<time_type, Event*>> mQueue;
		boost::mutex mMutex;
		volatile bool mAdvancing;
};

#endif // TIMEDEVENTQUEUE_H
