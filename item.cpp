#include "item.h"
#include "traits/itemtrait.h"


Item::Item() :
	mName(),
	mWeight(0),
	mSizeX(0),
	mSizeY(0),
	mSizeZ(0),
	mRoom(0) {

}

Item::Item(const std::string &name) :
	mName(name),
	mWeight(0),
	mSizeX(0),
	mSizeY(0),
	mSizeZ(0),
	mRoom(0) {

}

Item::~Item() {
	for (ItemTrait *trait : mTraits) delete trait;
}

void Item::initFromBase(const RHandle<Item> &b) {
	mBase = b;
	mName = b->name();
	mWeight = b->weight();
	mSizeX = b->sizeX();
	mSizeY = b->sizeY();
	mSizeZ = b->sizeZ();
	for (const std::pair<const std::string, ItemTrait *> &t : b->mTraits) {
		mTraits[t.second->traitName()] = t.second->clone();
	}
}

const std::string &Item::name() const {
	return mName;
}

Json::Value Item::serialize() const {
	Json::Value ret(Json::objectValue);
	if (mBase.isNull()) {
		ret["name"] = mName;
		ret["weight"] = mWeight;
		Json::Value size(Json::objectValue);
		size["x"] = mSizeX;
		size["y"] = mSizeY;
		size["z"] = mSizeZ;
		ret["size"] = size;
		if (!mTraits.empty()) {
			Json::Value traits(Json::objectValue);
			for (const std::pair<const std::string, ItemTrait *> &t : b->mTraits) {
				traits[t.second->traitName()] = t.second->serialize();
			}
		}
	}
	else {
		if (mName != mBase->name()) ret["name"] = mName;
		if (mWeight != mBase->weight()) ret["weight"] = mWeight;
		if (sizeX() != mBase->sizeX() || sizeY() != mBase->sizeY() || sizeZ() != mBase->sizeZ())  {
			Json::Value size(Json::objectValue);
			if (sizeX() != mBase->sizeX()) {
				size["x"] = mSizeX;
			}
			if (sizeY() != mBase->sizeY()) {
				size["y"] = mSizeY;
			}
			if (sizeX() != mBase->sizeX()) {
				size["x"] = mSizeX;
			}
		}


		size["y"] = mSizeY;
		size["z"] = mSizeZ;
		ret["size"] = size;
		if (!mTraits.empty()) {
			Json::Value traits(Json::objectValue);
			for (const std::pair<const std::string, ItemTrait *> &t : b->mTraits) {
				traits[t.second->traitName()] = t.second->serialize();
			}
		}
	}
	return ret;
}

bool Item::deserialize(const Json::Value &val) {

}
