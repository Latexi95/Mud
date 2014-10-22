#ifndef LANDMARKTRAIT_H
#define LANDMARKTRAIT_H
#include "itemtrait.h"

class LandmarkTrait : public ItemTrait {
	public:
		LandmarkTrait();
		~LandmarkTrait();
		Type type() const { return Landmark; }
};

#endif // LANDMARKTRAIT_H
