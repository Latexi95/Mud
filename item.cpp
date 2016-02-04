#include "item.h"
#include "traits/itemtrait.h"
#include "resourceservice.h"

Item::Item() :
    mWeight(0)
{
}

Item::Item(const std::string &id) :
    mId(id),
    mName(),
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
            for (const std::pair<const unsigned, std::unique_ptr<ItemTrait> > &t : mTraits) {
                traits[t.second->traitName()] = t.second->serialize();
            }
        }
    }
    else {
        ret["base"] = mBase->id();
        if (mName != mBase->name()) ret["name"] = mName.serialize();
        if (mWeight != mBase->weight()) ret["weight"] = mWeight;
        if (size() != mBase->size())  {
            ret["size"] = Json::serialize(size());
        }
        if (!mTraits.empty()) {
            Json::Value traits(Json::objectValue);
            for (const std::pair<const unsigned, std::unique_ptr<ItemTrait> > &t : mBase->mTraits) {
                if (t.second) {
                    traits[t.second->traitName()] = t.second->serialize();
                }
                else {
                    traits[t.second->traitName()] = Json::Value();
                }

            }
            ret["traits"] = traits;
        }
    }
    return ret;
}

void Item::deserialize(const Json::Value &val) {
    if (val.isObject()) {
        const Json::Value &base = val["base"];
        if (base.isString()) {
            std::shared_ptr<Item> b = ResourceService::instance()->baseItem(base.asString());
            if (!b) throw SerializationException("Can't find base item \"" + base.asString() + "\"");
            initFromBase(b);
        }
        mName.deserialize(val["name"]);

        Box<float> size;
        Json::deserialize(val["size"], size);
        setSize(size);
        setWeight(base.get("weight", weight()).asDouble());

        const Json::Value &traits = val["traits"];
        for (Json::Value::const_iterator i = traits.begin(); i != traits.end(); ++i) {

            if (i->isNull()) {
                std::unique_ptr<ItemTrait> trait = ItemTrait::createItemTraitByName(i.memberName());
                if (!trait) {
                    std::cerr << "Invalid trait name \"" << i.memberName() << "\"" << std::endl;
                    continue;
                }
                mTraits[(unsigned)trait->type()] = std::move(trait);
            }
            else {
                std::unique_ptr<ItemTrait> trait = ItemTrait::createItemTraitByName(i.memberName());
                if (!trait) {
                    std::cerr << "Invalid trait name \"" << i.memberName() << "\"" << std::endl;
                    continue;
                }
                assert(trait);
                trait->deserialize(*i);
                mTraits[(unsigned)trait->type()] = std::move(trait);
            }
        }
    }
    else if (val.isString()) {
        auto base = ResourceService::instance()->baseItem(val.asString());
        if (!base) {
            throw SerializationException("Can't find item \"" + val.asString() + "\"");
        }
        base->clone(*this);
    }
    else {
        throw SerializationException("Item::deserialize: expecting string or object value");
    }

}

bool Item::hasTrait(ItemTraitType type) {
    auto traitIt = mTraits.find((unsigned)type);
    if (traitIt != mTraits.end()) {
        if (traitIt->second) return true;
    }
    else if (mBase) {
        return mBase->hasTrait(type);
    }
    return false;
}

const ItemTrait &Item::trait(ItemTraitType type) {
    auto traitIt = mTraits.find((unsigned)type);
    if (traitIt != mTraits.end()) {
        if (traitIt->second)
            return *traitIt->second;
    }
    else if (mBase) {
        return mBase->trait(type);
    }
    assert("Item doesn't have a trait with that name" && 0);
    return mBase->trait(type);
}

void Item::clone(Item &copy) const
{
    copy.mBase = mBase;
    copy.mName = name();
    copy.mWeight = weight();
    copy.mSize = size();
    copy.mTraits.clear();
    for (const std::pair<const unsigned, std::unique_ptr<ItemTrait>> &t : mTraits) {
        copy.mTraits[t.first] = t.second->clone();
    }
}

std::unique_ptr<Item> Item::clone() const {
    std::unique_ptr<Item> copy(new Item());
    clone(*copy);
    return std::move(copy);
}

Json::Value Json::Serializer<Item>::serialize(const Item &i)
{
    return i.serialize();
}

void Json::Serializer<Item>::deserialize(const Json::Value &v, Item &i)
{
    i.deserialize(v);
}
