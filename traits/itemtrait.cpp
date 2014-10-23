#include "itemtrait.h"
#include "containertrait.h"
#include "landmarktrait.h"
#include "lockedtrait.h"
#include "weapontrait.h"

ItemTrait::ItemTrait() {
}

ItemTrait::~ItemTrait() {

}

ItemTrait *ItemTrait::createItemTraitByName(const std::string &name) {
	if (name == "container") {
		return new ContainerTrait();
	}
	if (name == "landmark") {
		return new LandmarkTrait();
	}
	if (name == "locked") {
		return new LockedTrait();
	}
	if (name == "weapon") {
		return new WeaponTrait();
	}
	return 0;
}

