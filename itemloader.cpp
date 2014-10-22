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
	std::unique_ptr<Resource<Item> > res(new Resource<Item>(filePath));
	Item &item = res->resource();
	bool valid =
			JsonChecker(base, filePath)
			.hasObjectElement("base", true)
			.moveTo("size", true)
				.hasNumberElement("x", true)
				.hasNumberElement("y", true)
				.hasNumberElement("z", true)
				.moveBack()
			.hasNumberElement("weight", true)
			.hasObjectElement("traits", true);
	if (!valid) {
		return 0;
	}
	const Json::Value &v = base["base"];
	if (!v.isNull()) {
		RHandle<Item> b = mResourceService->item(v.asString());
		if (b.isNull()) return 0;
		item.initFromBase(b);
	}
	const Json::Value &size = base["size"];
	if (!size.isNull()) {
		item.setSizeX(size.get("x", item.sizeX()));
		item.setSizeY(size.get("y", item.sizeY()));
		item.setSizeZ(size.get("z", item.sizeZ()));
	}
	item.setWeight(base.get("weight", item.weight()));
	return res.release();
}

bool ItemLoader::checkIsObject(const Json::Value &val, const char *name, const std::string &filePath) const {
	if (val.isObject()) return true;
	std::cerr << "Failed to load " << filePath << " : Json element \"" << name << "\" isn't an object\n";
	return false;
}
