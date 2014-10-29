#ifndef LANDMARKTRAIT_H
#define LANDMARKTRAIT_H
#include "itemtrait.h"

class LandmarkTrait : public ItemTrait {
	public:
		LandmarkTrait();
		~LandmarkTrait();
		Type type() const { return Landmark; }
		ItemTrait *clone() const;
		Json::Value serialize() const;
		bool deserialize(const Json::Value &val);
		bool hasToBeSerialized(const ItemTrait *base) const;
		const char *traitName() const { return "landmark"; }
};

#endif // LANDMARKTRAIT_H
