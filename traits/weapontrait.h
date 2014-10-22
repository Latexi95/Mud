#ifndef WEAPON_H
#define WEAPON_H
#include "itemtrait.h"
class WeaponTrait : public ItemTrait {
	public:
		WeaponTrait();
		~WeaponTrait();

		Type type() const { return Weapon; }
};

#endif // WEAPON_H
