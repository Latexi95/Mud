#ifndef GAMEEVENTLOOP_H
#define GAMEEVENTLOOP_H

#include <boost/lockfree/queue.hpp>
#include <boost/atomic.hpp>
#include <unordered_map>
class Event;
class EventHandler;
class GameEventLoop {
	public:
		GameEventLoop();
		~GameEventLoop();
		static GameEventLoop *instance();
		void push(Event *e);
		void loop();
		void handleEvent(Event *e);
		void stop();
		void addHandler(EventHandler *handler);
	private:
		boost::lockfree::queue<Event*> mEventQueue;
		volatile boost::atomic_bool mRunning;
		std::unordered_multimap<int, EventHandler*> mEventHandlers;
};

#endif // GAMEEVENTLOOP_H
