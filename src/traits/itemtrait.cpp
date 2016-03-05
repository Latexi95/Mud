#include "itemtrait.h"
#include "containertrait.h"
#include "landmarktrait.h"
#include "lockedtrait.h"
#include "weapontrait.h"
#include "eatabletrait.h"


ItemTrait::ItemTrait() {
}

ItemTrait::~ItemTrait() {

}

std::unique_ptr<ItemTrait> ItemTrait::createItemTraitByType(ItemTraitType type)
{
    switch (type) {
    case ItemTraitType::Container:
        return std::unique_ptr<ItemTrait>(new ContainerTrait());
    case ItemTraitType::Landmark:
        return std::unique_ptr<ItemTrait>(new LandmarkTrait());
    case ItemTraitType::Locked:
        return std::unique_ptr<ItemTrait>(new LockedTrait());
    case ItemTraitType::Weapon:
        return std::unique_ptr<ItemTrait>(new WeaponTrait());
    case ItemTraitType::Eatable:
        return std::unique_ptr<ItemTrait>(new EatableTrait());
    default:
        return std::unique_ptr<ItemTrait>();
    }
}

std::unique_ptr<ItemTrait> ItemTrait::createItemTraitByName(const std::string &name) {
    ItemTraitType type = ItemTraitTypeFromString(name);
    return createItemTraitByType(type);
}

void ItemTrait::serializeBase(Json::Value &val) const {
    val["traitname"] = traitName();
}


void ItemTraitVisitor::visit(ContainerTrait *t)
{
}

void ItemTraitVisitor::visit(LandmarkTrait *t)
{
}

void ItemTraitVisitor::visit(LockedTrait *t)
{
}

void ItemTraitVisitor::visit(WeaponTrait *t)
{
}

void ItemTraitVisitor::visit(EatableTrait *t)
{
}
