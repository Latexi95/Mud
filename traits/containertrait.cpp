#include "containertrait.h"
#include "jsonchecker.h"
#include "resourceservice.h"

ContainerTrait::ContainerTrait() :
	ItemTrait(),
	mContainerType(Open)
{
}

ContainerTrait::~ContainerTrait()
{

}

ItemTrait *ContainerTrait::clone() const {
	ContainerTrait *trait = new ContainerTrait();
	for (const RHandle<Item> &items : mContainedItems) {
		trait->mContainedItems.push_back(items->clone());
	}
	trait->mContainerType = this->mContainerType;
	return trait;
}

Json::Value ContainerTrait::serialize() const {
	Json::Value ret;
	switch (mContainerType) {
		case Open:
			ret["type"] = "open"; break;
		case Closed:
			ret["type"] = "closed"; break;
		case Opened:
			ret["type"] = "opened"; break;
	}
	Json::Value items(Json::arrayValue);
	for (RHandle<Item> item : mContainedItems) {
		if (item.path().empty()) {
			Json::Value iVal = item->serialize();
			if (!iVal.isNull()) {
				items.append(iVal);
			}
		}
		else {
			items.append(item.path());
		}
	}
	ret["items"] = items;
	return ret;
}

bool ContainerTrait::deserialize(const Json::Value &val) {
	if (!val.isObject()) return false;
	const Json::Value &type = val["type"];
	if (!type.isString()) return false;
	std::string typeStr = type.asString();
	if (typeStr == "open") {
		mContainerType = Open;
	} else if (typeStr == "closed") {
		mContainerType = Closed;
	} else if (typeStr == "opened") {
		mContainerType = Opened;
	} else {
		return false;
	}
	const Json::Value &items = val["items"];
	if (!items.isArray()) return false;
	for (Json::Value::const_iterator i = items.begin(); i != items.end(); i++) {
		if (i->isString()) {
			RHandle<Item> item = ResourceService::instance()->item(i->asString());
			if (item.isNull()) return false;
			mContainedItems.push_back(item);
		} else if (i->isObject()) {
			RHandle<Item> item = createDynamicResource<Item>();
			if (!item->deserialize(*i)) return false;
			mContainedItems.push_back(item);
		} else {
			return false;
		}
	}
	return true;
}

bool ContainerTrait::hasToBeSerialized(const ItemTrait *base) const {
	assert(base->type() == Container);
	const ContainerTrait *p = static_cast<const ContainerTrait*>(base);
	if (p->containedItems().empty() && this->containedItems().empty() && this->containerType() == p->containerType()) return false;
	return true;
}
