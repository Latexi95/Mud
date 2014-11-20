#include "mud.h"
#include "level.h"
Mud::Mud() :
	mEventLoopThread(0),
	mPlayerService(this) {
	mEventLoop.addHandler(&mCommandService);

}

Mud::~Mud() {
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
