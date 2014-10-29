#include "landmarktrait.h"

LandmarkTrait::LandmarkTrait() :
	ItemTrait()
{
}

LandmarkTrait::~LandmarkTrait() {

}

ItemTrait *LandmarkTrait::clone() const {
	LandmarkTrait *trait = new LandmarkTrait();

	return trait;
}

Json::Value LandmarkTrait::serialize() const {

}

bool LandmarkTrait::deserialize(const Json::Value &val) {

}

bool LandmarkTrait::hasToBeSerialized(const ItemTrait *base) const {
	return true;
}
