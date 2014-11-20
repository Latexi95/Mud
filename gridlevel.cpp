#include "gridlevel.h"
#include "room.h"

class GridLevelRoom : public Room {
	public:
		GridLevelRoom(GridLevel *gridLevel);
		virtual ~GridLevelRoom();
		virtual std::vector<RHandle<Item>> items() const;
		virtual std::vector<RHandle<Character>> characters() const;
};



GridLevel::GridLevel(const std::string &name, int width, int height) :
	Level(name),
	mWidth(width),
	mHeight(height) {
	mRooms = new Room*[width * height];
	std::fill(mRooms, mRooms + (width * height), nullptr);
}

GridLevel::~GridLevel() {
	delete[] mRooms;
}

Room &GridLevel::room(int localX, int localY) {
	Room *room = mRooms[localX + localY * mWidth];
	if (!room) {
		mRooms[localX + localY * mWidth] = new GridLevelRoom(this);
	}
	return room;
}


GridLevelRoom::GridLevelRoom(GridLevel *gridLevel) :
	Room(gridLevel) {

}

GridLevelRoom::~GridLevelRoom() {

}

std::vector<RHandle<Item> > GridLevelRoom::items() const {

}

std::vector<RHandle<Character> > GridLevelRoom::characters() const {

}
