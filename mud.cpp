#include "mud.h"

Mud::Mud() :
	mEventLoopThread(0) {
	mEventLoop.addHandler(&mCommandService);

}

Mud::~Mud() {
	for (Level *r : mRooms) {
		delete r;
	}
}

void Mud::start() {
	assert(!mEventLoopThread);
	mEventLoopThread = new boost::thread([&]() {
		mEventLoop.loop();
	});
}

void Mud::stop() {
	mEventLoop.stop();
	mEventLoopThread->join();
	delete mEventLoopThread;
	mEventLoopThread = 0;
}
