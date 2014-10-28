#include "item.h"
#include "traits/itemtrait.h"
#include "resourceservice.h"

Item::Item() :
	mName(),
	mWeight(0),
	mSizeX(0),
	mSizeY(0),
	mSizeZ(0){

}

Item::Item(const Name &name) :
	mName(name),
	mWeight(0),
	mSizeX(0),
	mSizeY(0),
	mSizeZ(0){

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
		mTraits[t.first] = t.second->clone();
	}
}

const Name &Item::name() const {
	return mName;
}

void Item::setName(const Name &n) {
	mName = n;
}

Json::Value Item::serialize() const {
	Json::Value ret(Json::objectValue);
	if (mBase.isNull()) {
		ret["name"] = mName.serialize();
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
		ret["base"] = mBase.path();
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
			if (sizeZ() != mBase->sizeZ()) {
				size["z"] = mSizeZ;
			}

			ret["size"] = size;
		}
		if (!mTraits.empty()) {
			Json::Value traits(Json::objectValue);
			for (const std::pair<const std::string, ItemTrait *> &t : mBase->mTraits) {
				if (mTraits.find(t.first) == mTraits.end()) {
					traits[t.first] = Json::Value();
				} else{
					ItemTrait *baseTrait = t.second;
					ItemTrait *trait = mTraits[t.first];
					if (trait->hasToBeSerialized(baseTrait)) {
						traits[t.first] = trait->serialize();
					}
				}
			}
		}
	}
	return ret;
}

bool Item::deserialize(const Json::Value &val) {
	const Json::Value &base = val["base"];
	if (base.isString()) {
		 RHandle<Item> b = ResourceService::instance()->item(base.asString());
		 if (b.isNull()) return false;
		 initFromBase(b);
	}
	if (!mName.deserialize(base["name"])) return false;

	const Json::Value &size = base["size"];
	if (!size.isNull()) {
		setSizeX(size.get("x", item.sizeX()));
		setSizeY(size.get("y", item.sizeY()));
		setSizeZ(size.get("z", item.sizeZ()));
	}
	setWeight(base.get("weight", item.weight()));

	const Json::Value &traits = val["traits"];
	for (Json::Value::const_iterator i = traits.begin(); traits.end(); ++i) {
		if (mTraits.find(i.memberName()) != mTraits.end()) {
			if (i->isNull()) {
				delete mTraits[i.memberName()];
				mTraits.erase(i.memberName());
			}
			else {
				ItemTrait *trait = ItemTrait::createItemTraitByName(i.memberName());
				assert(trait);
				if (trait->deserialize(*i)) {
					delete mTraits[i.memberName()];
					mTraits[i.memberName()] = trait;
				}
				else {
					std::cerr << "Failed to deserialize trait \"" << i.memberName() << "\"" << std::endl;
					delete trait;
				}
			}
		}
		else {
			ItemTrait *trait = ItemTrait::createItemTraitByName(i.memberName());
			if (!trait) {
				std::cerr << "Invalid trait name \"" << i.memberName() << "\"" << std::endl;
				continue;
			}
			if (trait->deserialize(*i)) {
				mTraits[i.memberName()] = trait;
			}
			else {
				std::cerr << "Failed to deserialize trait \"" << i.memberName() << "\"" << std::endl;
				delete trait;
			}
		}
	}
	return true;
}
