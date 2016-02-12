#ifndef WEAPON_H
#define WEAPON_H
#include "itemtrait.h"
class WeaponTrait : public ItemTrait {
public:
    WeaponTrait();
    ~WeaponTrait();

    ItemTraitType type() const { return ItemTraitType::Weapon; }
    std::unique_ptr<ItemTrait> clone() const;
    Json::Value serialize() const;
    void deserialize(const Json::Value &val);
    const char *traitName() const { return "weapon"; }
};

#endif // WEAPON_H
