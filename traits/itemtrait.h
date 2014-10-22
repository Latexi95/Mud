#ifndef ITEMTRAIT_H
#define ITEMTRAIT_H
#include "jsonserializable.h"
class ItemTrait : public JsonSerializable {
	public:
		enum Type {
			Weapon,
			Container,
			Locked,
			Landmark
		};

		ItemTrait();
		virtual ~ItemTrait();
		virtual ItemTrait *clone() const = 0;
		virtual Type type() const = 0;
		virtual bool hasToBeSerialized(const ItemTrait *inheritedVersion) = 0;
		const char *traitName() const = 0;
};

#endif // ITEMTRAIT_H
