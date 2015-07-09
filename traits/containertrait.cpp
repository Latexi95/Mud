#include "containertrait.h"
#include "jsonchecker.h"
#include "resourceservice.h"
#include <boost/algorithm/string.hpp>

ContainerTrait::ContainerTrait() :
    ItemTrait(),
    mContainerType(Open)
{
}

ContainerTrait::~ContainerTrait()
{

}

std::unique_ptr<ItemTrait> ContainerTrait::clone() const {
    std::unique_ptr<ContainerTrait> trait = std::unique_ptr<ContainerTrait>(new ContainerTrait());
    for (const std::unique_ptr<Item> &items : mContainedItems) {
        trait->mContainedItems.push_back(items->clone());
    }
    trait->mContainerType = this->mContainerType;
    return std::move(trait);
}

Json::Value ContainerTrait::serialize() const {
    Json::Value ret;
    serializeBase(ret);
    ret["type"] = Json::serialize(mContainerType);
    ret["items"] = Json::serialize(mContainedItems);
    return ret;
}

void ContainerTrait::deserialize(const Json::Value &val) {
    if (!val.isObject()) {
        throw SerialiazationException("Expecting json object ContainerTrait");
    }

    const Json::Value &type = val["type"];

    Json::deserialize(type, mContainerType);

    const Json::Value &items = val["items"];
    if (!items.isArray()) {
        throw SerialiazationException("Expecting an array of items in ContainerTrait::items");
    }
    for (Json::Value::const_iterator i = items.begin(); i != items.end(); i++) {
        if (i->isString()) {
            std::unique_ptr<Item> item = ResourceService::instance()->item(i->asString());
            mContainedItems.push_back(std::move(item));
        } else if (i->isObject()) {
            std::unique_ptr<Item> item = std::unique_ptr<Item>(new Item());
            item->deserialize(*i);
            mContainedItems.push_back(std::move(item));
        } else {
            assert("Invalid item json" && 0);
        }
    }
}

