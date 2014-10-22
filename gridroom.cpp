#include "gridroom.h"


GridRoom::Cell::Cell() {

}

GridRoom::Cell::~Cell() {

}

bool GridRoom::Cell::isMovableLocation() const {
	return !mWall;
}

GridRoom::GridRoom(const std::string &name, int w, int h) :
	Room(name),
	mWidth(w),
	mHeight(h),
	mGrid(new Cell[w * h])
{
}

GridRoom::~GridRoom() {
	delete[] mGrid;
}

bool GridRoom::isMovableLocation(int x, int y) const {
	if (!locationInGrid(x, y)) return false;
	return cell(x, y).isMovableLocation();
}

std::vector<Item *> GridRoom::itemsInLocation(int x, int y) const {
	if (!locationInGrid(x, y)) return std::vector<Item *>();
	return cell(x, y).items();
}

bool GridRoom::locationInGrid(int x, int y) const {
	return x >= 0 && x < mWidth && y >= 0 && y < mHeight;
}

GridRoom::Cell &GridRoom::cell(int x, int y) {
	return mGrid[x + y * mWidth];
}

const GridRoom::Cell &GridRoom::cell(int x, int y) const {
	return mGrid[x + y * mWidth];
}


