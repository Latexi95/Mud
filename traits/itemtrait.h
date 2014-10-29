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
		virtual bool hasToBeSerialized(const ItemTrait *base) const = 0;
		virtual const char *traitName() const = 0;

		static ItemTrait *createItemTraitByName(const std::string &name);
};

#endif // ITEMTRAIT_H
