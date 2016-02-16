#include "weapontrait.h"

WeaponTrait::WeaponTrait() :
    ItemTrait()
{
}

WeaponTrait::~WeaponTrait() {

}

std::unique_ptr<ItemTrait> WeaponTrait::clone() const {
    std::unique_ptr<ItemTrait> trait = std::unique_ptr<ItemTrait>(new WeaponTrait());
    return trait;
}

Json::Value WeaponTrait::serialize() const {
    return Json::Value();
}

void WeaponTrait::deserialize(const Json::Value &val) {

}

void WeaponTrait::accept(ItemTraitVisitor *visitor)
{
    visitor->visit(this);
}

