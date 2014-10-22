#include "resourceservice.h"
#include <json/json.h>
#include <fstream>
#include "itemloader.h"
ResourceService::ResourceService() :
	mItemStash(ItemLoader(this))
{
}

ResourceService::~ResourceService()
{

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
