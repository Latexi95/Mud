#include "resourceservice.h"
#include <json/json.h>
#include <fstream>
#include <cassert>
#include "item.h"
#include "character.h"
#include "jsonserializableloader.h"
#include "player.h"

static ResourceService *sInstance = 0;

ResourceService::ResourceService() :
	mItemStash(),
	mCharacterStash(){
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
	file.open(path);
	if (!file.is_open()) {
		return Json::Value();
	}
	Json::Reader reader;

	if (!reader.parse(file, ret, false)) {
		std::cerr << reader.getFormattedErrorMessages() << std::endl;
		return Json::Value();
	}

	return ret;
}

RHandle<Item> ResourceService::item(const std::string &path) {
	return mItemStash.get("data/" + path + ".json", JsonSerializableLoader<Item>());
}

RHandle<Character> ResourceService::character(const std::string &path) {
	return mCharacterStash.get("data/" + path + ".json", JsonSerializableLoader<Character>());
}

RHandle<Player> ResourceService::player(const std::string &name) {
	return mPlayerStash.get("data/players/" + name + ".json", JsonSerializableLoader<Player>());
}

RHandle<Player> ResourceService::createPlayer(const std::string &name) {
	return mPlayerStash.create("data/players/" + name + ".json");
}

void ResourceService::save(RHandle<Player> player) {
	Json::Value json = player->serialize();
	Json::StyledStreamWriter writer;
	std::ofstream file(player.path());
	std::cout << "Saving a player to file " << player.path() << std::endl;
	writer.write(file, json);
	file.close();
}
