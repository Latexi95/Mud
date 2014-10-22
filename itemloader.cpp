#include "itemloader.h"
#include "jsonchecker.h"
ItemLoader::ItemLoader(ResourceService *resServ) :
	mResourceService(resServ)
{
}

ItemLoader::~ItemLoader() {

}

Resource<Item> *ItemLoader::operator()(const std::string &filePath) {
	Json::Value base = mResourceService->requestJsonResource(filePath);
	if (!base.isObject()) return 0;
	std::unique_ptr<Resource<Item> > res(new Resource<Item>());
	Item &item = res->resource();
	bool valid =
			JsonChecker(base, filePath)
			.hasObjectElement("base", true)
			.moveTo("size", true)
				.hasNumberElement("x", true)
				.hasNumberElement("y", true)
				.hasNumberElement("z", true)
				.moveBack()

			;
	if (!valid) {

	}
	const Json::Value &v = base["base"];
	if (v.isString()) {
		RHandle<Item> b = mResourceService->item(v.asString());
		if (b.isNull()) return 0;
		item.initFromBase();
	}

	const Json::Value &size = base["size"];
	if (!checkIsObject(size)) return 0;
	item.setWidth();
	return res.release();
}

bool ItemLoader::checkIsObject(const Json::Value &val, const char *name, const std::string &filePath) const {
	if (val.isObject()) return true;
	std::cerr << "Failed to load " << filePath << " : Json element \"" << name << "\" isn't an object\n";
	return false;
}
