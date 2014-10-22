#ifndef LOCKEDTRAIT_H
#define LOCKEDTRAIT_H
#include "itemtrait.h"
class LockedTrait : public ItemTrait  {
	public:
		LockedTrait();

		Type type() const { return Locked; }
};

#endif // LOCKEDTRAIT_H
