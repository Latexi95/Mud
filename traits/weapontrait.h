#ifndef WEAPON_H
#define WEAPON_H
#include "itemtrait.h"
class WeaponTrait : public ItemTrait {
	public:
		WeaponTrait();
		~WeaponTrait();

		Type type() const { return Weapon; }
		ItemTrait *clone() const;
		Json::Value serialize() const;
		bool deserialize(const Json::Value &val);
		bool hasToBeSerialized(const ItemTrait *base) const;
		const char *traitName() const { return "weapon"; }
};

#endif // WEAPON_H
