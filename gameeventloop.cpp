#include "gameeventloop.h"
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include "event.h"
#include "eventhandler.h"
static GameEventLoop *sInstance = 0;

GameEventLoop::GameEventLoop() :
	mEventQueue(100),
	mRunning(false) {
	assert(!sInstance);
	sInstance = this;
}

GameEventLoop::~GameEventLoop() {
	sInstance = 0;
}

GameEventLoop *GameEventLoop::instance() {
	return sInstance;
}

void GameEventLoop::push(Event *e) {
	bool success = mEventQueue.push(e);
	assert(success);
}

void GameEventLoop::loop() {
	mRunning = true;
	Event *e;
	while (mRunning.load(boost::memory_order_acquire)) {
		if (mEventQueue.pop(e)) {
			handleEvent(e);
		}
		else {
			boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
		}
	}

}

void GameEventLoop::handleEvent(Event *e) {
	auto handlers = mEventHandlers.equal_range(e->type());
	for (auto i = handlers.first; i != handlers.second; i++) {
		i->second->handleEvent(e);
		if (e->accepted()) break;
	}
	if (!e->accepted()) delete e;
}

void GameEventLoop::stop() {
	mRunning.store(false, boost::memory_order_release);
}

void GameEventLoop::addHandler(EventHandler *handler) {
	std::vector<int> handledEventTypes = handler->handledEventTypes();
	for (int i : handledEventTypes) {
		mEventHandlers.insert(std::pair<int, EventHandler*>(i, handler));
	}
}
