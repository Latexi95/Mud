#include "lockedtrait.h"

LockedTrait::LockedTrait() :
	ItemTrait()
{
}

ItemTrait *LockedTrait::clone() const {
	LockedTrait *trait = new LockedTrait();

	return trait;
}

Json::Value LockedTrait::serialize() const {

}

bool LockedTrait::deserialize(const Json::Value &val) {

}

bool LockedTrait::hasToBeSerialized(const ItemTrait *base) const {
	return true;
}
