#include "item.h"



Item::Item() :
	mName(),
	mWeight(0),
	mWidth(0),
	mHeight(0),
	mDepth(0),
	mRoom(0) {

}

Item::Item(const std::string &name) :
	mName(name),
	mWeight(0),
	mWidth(0),
	mHeight(0),
	mDepth(0),
	mRoom(0) {

}

Item::~Item() {
	for (ItemTrait *trait : mTraits) delete trait;
}

void Item::initFromBase(const RHandle<Item> &b) {
	mBase = b;
	mName = b->name();
	mWeight = b->weight();
	mWidth = b->sizeX();
	mHeight = b->sizeY();
	mDepth = b->setSizeX();
	for (ItemTrait *t : b->mTraits) {
		mTraits.push_back(t->clone());
	}
}

const std::string &Item::name() const {
	return mName;
}
