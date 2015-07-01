#include "landmarktrait.h"

LandmarkTrait::LandmarkTrait() :
	ItemTrait()
{
}

LandmarkTrait::~LandmarkTrait() {

}

std::unique_ptr<ItemTrait> LandmarkTrait::clone() const {
	std::unique_ptr<LandmarkTrait> trait = std::unique_ptr<LandmarkTrait>(new LandmarkTrait());

	return std::move(trait);
}

Json::Value LandmarkTrait::serialize() const {

}

void LandmarkTrait::deserialize(const Json::Value &val) {

}
