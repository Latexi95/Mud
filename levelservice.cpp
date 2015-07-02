#include "levelservice.h"
#include "resourceservice.h"

LevelService *LS = 0;

LevelService::LevelService() {
	assert(LS == 0);
	LS = this;
}

LevelService::~LevelService() {
	assert(LS);
	LS = 0;
}

LevelService *LevelService::instance() {
	assert(LS);
	return LS;
}

std::shared_ptr<Level> LevelService::level(const std::string &levelId) {
	boost::unique_lock<boost::mutex> lock(mMutex);
	auto levelIt = mLevels.find(levelId);
	if (levelIt != mLevels.end()) return levelIt->second;

	Json::Value levelJson = RS->readJsonFile("data/levels/" + levelId + ".json");

	std::shared_ptr<Level> level = std::make_shared<Level>(levelId);
	level->deserialize(levelJson);
	mLevels[levelId] = level;
	return level;
}
