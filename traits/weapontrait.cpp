#include "weapontrait.h"

WeaponTrait::WeaponTrait() :
	ItemTrait()
{
}

WeaponTrait::~WeaponTrait() {

}

ItemTrait *WeaponTrait::clone() const {
	WeaponTrait *trait = new WeaponTrait();
	return trait;
}

Json::Value WeaponTrait::serialize() const {

}

bool WeaponTrait::deserialize(const Json::Value &val) {

}

bool WeaponTrait::hasToBeSerialized(const ItemTrait *base) const {
	return true;
}
