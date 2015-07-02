#include "resourceservice.h"
#include <json/json.h>
#include <fstream>
#include <cassert>
#include "item.h"
#include "character.h"
#include "player.h"
#include "textgen/textutils.h"


ResourceService *RS = 0;

ResourceService::ResourceService() {
	assert(RS == 0);
	RS = this;
}

ResourceService::~ResourceService() {
	RS = 0;
}

ResourceService *ResourceService::instance() {
	assert(RS);
	return RS;
}

Json::Value ResourceService::readJsonFile(const std::string &path) const {
	Json::Value ret;
	std::ifstream file(path);
	if (!file) {
		std::cerr << "Failed to open file " << path << std::endl;
		return Json::Value();
	}
	Json::Reader reader;

	if (!reader.parse(file, ret, false)) {
		std::cerr << reader.getFormattedErrorMessages() << std::endl;
		return Json::Value();
	}

	return ret;
}

bool ResourceService::saveJsonFile(const std::string &path, const Json::Value &val) const {
	std::ofstream file(path);
	if (!file) {
		std::cerr << "Failed to write file " << path << std::endl;
		return false;
	}

	Json::StyledStreamWriter writer;
	writer.write(file, val);
	return true;
}

std::unique_ptr<Item> ResourceService::item(const std::string &path) {
	std::shared_ptr<Item> base = baseItem(path);
	if (!base) return std::unique_ptr<Item>();

	return base->clone();
}

std::shared_ptr<Item> ResourceService::baseItem(const std::string &path) {
	boost::unique_lock<boost::mutex> lock(mItemMutex);
	auto itemIt = mBaseItems.find(path);
	if (itemIt != mBaseItems.end()) return itemIt->second;

	Json::Value v = readJsonFile("data/" + path);
	if (v.isNull()) return nullptr;

	std::shared_ptr<Item> item = std::make_shared<Item>(path);
	item->deserialize(v);
	mBaseItems[path] = item;
	return item;
}


