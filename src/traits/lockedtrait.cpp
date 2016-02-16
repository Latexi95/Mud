#include "lockedtrait.h"

LockedTrait::LockedTrait() :
    ItemTrait()
{
}

LockedTrait::~LockedTrait()
{

}

std::unique_ptr<ItemTrait> LockedTrait::clone() const {
    std::unique_ptr<ItemTrait> trait = std::unique_ptr<ItemTrait>(new LockedTrait());

    return trait;
}

Json::Value LockedTrait::serialize() const {
    return Json::Value();
}

void LockedTrait::deserialize(const Json::Value &val) {

}

void LockedTrait::accept(ItemTraitVisitor *visitor)
{
    visitor->visit(this);
}
