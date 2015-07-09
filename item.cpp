#include "item.h"
#include "traits/itemtrait.h"
#include "resourceservice.h"

Item::Item() :
    mWeight(0)
{
}

Item::Item(const std::string &path) :
    mPath(path),
    mName(),
    mWeight(0) {

}

Item::Item(const Name &name) :
    mName(name),
    mWeight(0) {

}

Item::~Item() {
}

void Item::initFromBase(const std::shared_ptr<Item> &b) {
    mBase = b;
    mName = b->name();
    mWeight = b->weight();
    mSize = b->size();
}

const Name &Item::name() const {
    return mName;
}

void Item::setName(const Name &n) {
    mName = n;
}

Json::Value Item::serialize() const {
    Json::Value ret(Json::objectValue);
    if (!mBase) {
        ret["name"] = mName.serialize();
        ret["weight"] = mWeight;
        ret["size"] = serialize();
        if (!mTraits.empty()) {
            Json::Value traits(Json::objectValue);
            for (const std::pair<const std::string, std::unique_ptr<ItemTrait> > &t : mTraits) {
                traits[t.second->traitName()] = t.second->serialize();
            }
        }
    }
    else {
        ret["base"] = mBase->path();
        if (mName != mBase->name()) ret["name"] = mName.serialize();
        if (mWeight != mBase->weight()) ret["weight"] = mWeight;
        if (size() != mBase->size())  {
            ret["size"] = Json::serialize(size());
        }
        if (!mTraits.empty()) {
            Json::Value traits(Json::objectValue);
            for (const std::pair<const std::string, std::unique_ptr<ItemTrait> > &t : mBase->mTraits) {
                if (t.second) {
                    traits[t.first] = t.second->serialize();
                }
                else {
                    traits[t.first] = Json::Value();
                }

            }
            ret["traits"] = traits;
        }
    }
    return ret;
}

void Item::deserialize(const Json::Value &val) {
    const Json::Value &base = val["base"];
    if (base.isString()) {
        std::shared_ptr<Item> b = ResourceService::instance()->baseItem(base.asString());
        if (!b) throw SerialiazationException("Can't find base item \"" + base.asString() + "\"");
        initFromBase(b);
    }
    mName.deserialize(base["name"]);

    Box<double> size;
    Json::deserialize(base["size"], size);
    setSize(size);
    setWeight(base.get("weight", weight()).asDouble());

    const Json::Value &traits = val["traits"];
    for (Json::Value::const_iterator i = traits.begin(); i != traits.end(); ++i) {

        if (i->isNull()) {
            mTraits[i.memberName()] = std::unique_ptr<ItemTrait>();
        }
        else {
            std::unique_ptr<ItemTrait> trait = ItemTrait::createItemTraitByName(i.memberName());
            if (!trait) {
                std::cerr << "Invalid trait name \"" << i.memberName() << "\"" << std::endl;
                continue;
            }
            assert(trait);
            trait->deserialize(*i);
            mTraits[i.memberName()] = std::move(trait);
        }
    }
}

bool Item::hasTrait(const std::string &name) {
    auto traitIt = mTraits.find(name);
    if (traitIt != mTraits.end()) {
        if (traitIt->second) return true;
    }
    else if (mBase) {
        return mBase->hasTrait(name);
    }
    return false;
}

const ItemTrait &Item::trait(const std::string &name) {
    auto traitIt = mTraits.find(name);
    if (traitIt != mTraits.end()) {
        if (traitIt->second)
            return *traitIt->second;
    }
    else if (mBase) {
        return mBase->trait(name);
    }
    assert("Ite doesn't have a trait with that name" && 0);
    return mBase->trait(name);
}

std::unique_ptr<Item> Item::clone() const {
    std::unique_ptr<Item> copy(new Item(name()));
    copy->mBase = mBase;
    copy->mName = name();
    copy->mWeight = weight();
    copy->mSize = size();
    for (const std::pair<const std::string, std::unique_ptr<ItemTrait>> &t : mTraits) {
        copy->mTraits[t.first] = t.second->clone();
    }
    return std::move(copy);
}
