#include "containertrait.h"
#include "resourceservice.h"
#include <boost/algorithm/string.hpp>

ContainerTrait::ContainerTrait() :
    ItemTrait(),
    mContainerType(ContainerState::Open)
{
}

ContainerTrait::~ContainerTrait()
{

}

std::unique_ptr<ItemTrait> ContainerTrait::clone() const {
    std::unique_ptr<ContainerTrait> trait = std::unique_ptr<ContainerTrait>(new ContainerTrait());
    trait->mContainerType = this->mContainerType;
    return std::move(trait);
}

Json::Value ContainerTrait::serialize() const {
    Json::Value ret;
    serializeBase(ret);
    ret["type"] = Json::serialize(mContainerType);
    return ret;
}

void ContainerTrait::deserialize(const Json::Value &val) {
    if (!val.isObject()) {
        throw SerializationException("Expecting json object ContainerTrait");
    }

    const Json::Value &type = val["type"];

    Json::deserialize(type, mContainerType);
}

void ContainerTrait::accept(ItemTraitVisitor *visitor)
{
    visitor->visit(this);
}

