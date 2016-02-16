#include "itemtrait.h"
#include "containertrait.h"
#include "landmarktrait.h"
#include "lockedtrait.h"
#include "weapontrait.h"


ItemTrait::ItemTrait() {
}

ItemTrait::~ItemTrait() {

}

std::unique_ptr<ItemTrait> ItemTrait::createItemTraitByName(const std::string &name) {
    if (name == "container") {
        return std::unique_ptr<ItemTrait>(new ContainerTrait());
    }
    if (name == "landmark") {
        return std::unique_ptr<ItemTrait>(new LandmarkTrait());
    }
    if (name == "locked") {
        return std::unique_ptr<ItemTrait>(new LockedTrait());
    }
    if (name == "weapon") {
        return std::unique_ptr<ItemTrait>(new WeaponTrait());
    }
    return std::unique_ptr<ItemTrait>();
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
