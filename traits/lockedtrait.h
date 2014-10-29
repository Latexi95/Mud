#ifndef LOCKEDTRAIT_H
#define LOCKEDTRAIT_H
#include "itemtrait.h"
class LockedTrait : public ItemTrait  {
	public:
		LockedTrait();

		Type type() const { return Locked; }
		ItemTrait *clone() const;
		Json::Value serialize() const;
		bool deserialize(const Json::Value &val);
		bool hasToBeSerialized(const ItemTrait *base) const;
		const char *traitName() const { return "locked"; }
};

#endif // LOCKEDTRAIT_H
