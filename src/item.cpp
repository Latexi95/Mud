#include "item.h"
#include "traits/itemtrait.h"
#include "resourceservice.h"

BaseItem::BaseItem() :
    mWeight(0)
{
}

BaseItem::BaseItem(const std::string &id) :
    mId(id),
    mName(),
    mWeight(0) {

}

BaseItem::~BaseItem() {
}

void BaseItem::initFromBase(const std::shared_ptr<Item> &b) {
    mBase = b;
    mName = b->name();
    mWeight = b->weight();
    mSize = b->size();
}

const Name &BaseItem::name() const {
    return mName;
}

void BaseItem::setName(const Name &n) {
    mName = n;
}

Json::Value BaseItem::serialize() const {
    Json::Value ret(Json::objectValue);
    ret["name"] = Json::serialize(mName);
    ret["weight"] = mWeight;
    ret["size"] = Json::serialize(mSize);
    if (!mTraits.empty()) {
        Json::Value traits(Json::objectValue);
        for (const std::pair<const unsigned, std::unique_ptr<ItemTrait> > &t : mTraits) {
            traits[t.second->traitName()] = t.second->serialize();
        }
    }
    return ret;
}

void BaseItem::deserialize(const Json::Value &val) {
    if (!val.isObject()) {
        throw SerializationException("BaseItem::deserialize: expecting object value");
    }

    mName.deserialize(val["name"]);

    Json::deserialize(val["size"], mSize);
    Json::deserialize(val["weight"], mWeight);

    const Json::Value &traits = val["traits"];
    for (Json::Value::const_iterator i = traits.begin(); i != traits.end(); ++i) {
        std::unique_ptr<ItemTrait> trait = ItemTrait::createItemTraitByName(i.name());
        if (!trait) {
            std::cerr << "Invalid trait name \"" << i.name() << "\"" << std::endl;
            continue;
        }
        if (i->isNull()) {
            mTraits[(unsigned)trait->type()] = std::move(trait);
        }
        else {
            trait->deserialize(*i);
            mTraits[(unsigned)trait->type()] = std::move(trait);
        }
    }

}

bool BaseItem::hasTrait(ItemTraitType type) {
    auto traitIt = mTraits.find((unsigned)type);
    if (traitIt != mTraits.end()) {
        if (traitIt->second) return true;
    }
    return false;
}

ItemTrait *BaseItem::trait(ItemTraitType type) {
    auto traitIt = mTraits.find((unsigned)type);
    if (traitIt != mTraits.end()) {
        if (traitIt->second)
            return traitIt->second.get();
    }
    return nullptr;
}

void BaseItem::addTrait(std::unique_ptr<ItemTrait> &&trait)
{
    mTraits[(unsigned)trait->type()] = std::move(trait);
}

std::unique_ptr<ItemTrait> BaseItem::removeTrait(ItemTraitType type)
{
    auto it = mTraits.find((unsigned)type);
    if (it == mTraits.end()) return std::unique_ptr<ItemTrait>();

    std::unique_ptr<ItemTrait> trait = std::move(it->second);
    mTraits.erase(it);
    return std::move(trait);
}

const std::unordered_map<unsigned, std::unique_ptr<ItemTrait> > &BaseItem::traits() const
{
    return mTraits;
}

void BaseItem::clone(BaseItem &copy) const
{
    copy.mName = name();
    copy.mWeight = weight();
    copy.mSize = size();
    copy.mTraits.clear();
    for (const std::pair<const unsigned, std::unique_ptr<ItemTrait>> &t : mTraits) {
        copy.mTraits[t.first] = t.second->clone();
    }
}

std::unique_ptr<BaseItem> BaseItem::clone() const {
    std::unique_ptr<BaseItem> copy(new BaseItem());
    clone(*copy);
    return std::move(copy);
}

Json::Value Json::Serializer<BaseItem>::serialize(const BaseItem &i)
{
    return i.serialize();
}

void Json::Serializer<BaseItem>::deserialize(const Json::Value &v, BaseItem &i)
{
    i.deserialize(v);
}

Item::Item(const std::shared_ptr<BaseItem> &base) :
    mBase(base)
{

}

Item::~Item()
{

}
