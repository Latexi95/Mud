#include "lockedtrait.h"

LockedTrait::LockedTrait() :
    ItemTrait()
{
}

std::unique_ptr<ItemTrait> LockedTrait::clone() const {
    std::unique_ptr<ItemTrait> trait = std::unique_ptr<ItemTrait>(new LockedTrait());

    return trait;
}

Json::Value LockedTrait::serialize() const {

}

void LockedTrait::deserialize(const Json::Value &val) {

}
