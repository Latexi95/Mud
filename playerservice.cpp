#include "playerservice.h"
#include "resourceservice.h"
#include <cassert>
#include "mud.h"

static PlayerService *sInstance = 0;

PlayerService::PlayerService(Mud *mud) :
	mMud(mud) {
	assert(!sInstance);
	sInstance = this;
}

PlayerService::~PlayerService() {
	sInstance = 0;
}

PlayerService *PlayerService::instance() {
	assert(sInstance);
	return sInstance;
}

RHandle<Player> PlayerService::findPlayerByName(const std::string &name) {
	return ResourceService::instance()->player(name);
}

RHandle<Player> PlayerService::createPlayer(const std::string &name) {
	return ResourceService::instance()->createPlayer(name);
}
