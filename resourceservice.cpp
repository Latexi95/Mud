#include "resourceservice.h"
#include <json/json.h>
#include <fstream>
#include <cassert>
#include "item.h"
#include "character.h"
#include "jsonserializableloader.h"

ResourceService *sInstance = 0;

ResourceService::ResourceService() :
	mItemStash(JsonSerializableLoader<Item>()),
	mCharacterStash(JsonSerializableLoader<Character>()){
	assert(sInstance == 0);
	sInstance = this;
}

ResourceService::~ResourceService() {
	sInstance = 0;
}

ResourceService *ResourceService::instance() {
	assert(sInstance);
	return sInstance;
}

Json::Value ResourceService::requestJsonResource(const std::string &path) const {
	Json::Value ret;
	std::ifstream file;
	if (!file.open(path)) {
		std::cerr << "Can't open file " << path << std::endl;
		return ret;
	}
	Json::Reader reader;

	if (!reader.parse(file, ret, false)) {
		std::cerr << reader.getFormattedErrorMessages() << std::endl;
		return Json::Value();
	}

	return ret;
}

RHandle<Item> ResourceService::item(const std::string &path) {
	return mItemStash.get(path);
}

RHandle<Character> ResourceService::character(const std::string &path) {

}
