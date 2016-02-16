#ifndef WEAPON_H
#define WEAPON_H
#include "itemtrait.h"
class WeaponTrait : public ItemTrait {
public:
    static const ItemTraitType staticTraitType = ItemTraitType::Weapon;

    WeaponTrait();
    virtual ~WeaponTrait();

    virtual ItemTraitType type() const { return ItemTraitType::Weapon; }
    virtual std::unique_ptr<ItemTrait> clone() const;
    virtual Json::Value serialize() const;
    virtual void deserialize(const Json::Value &val);
    virtual const char *traitName() const { return "weapon"; }
    virtual void accept(ItemTraitVisitor *visitor);
};

#endif // WEAPON_H
