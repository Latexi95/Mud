#include "gridlevel.h"
#include "room.h"
class GridLevelRoom;
class GridLevelWall : public Wall {
	public:
		GridLevelWall(Wall::Side side, const GridLevelRoom *room, GridLevel::WallData *wallData);
		~GridLevelWall();
		const std::vector<std::unique_ptr<Item> > &items() const;
		bool noWall() const;

	private:
		GridLevel::WallData *wallData() const { return static_cast<GridLevel::WallData*>(mData); }
};

class GridLevelRoom : public Room {
	public:
		GridLevelRoom(int x, int y, GridLevel *gridLevel);
		~GridLevelRoom();
		std::vector<std::pair<Item *, Wall::Side>> items() const;
		std::vector<std::shared_ptr<Character>> characters() const;
		Wall wall(Wall::Side side) const;
		const std::vector<std::unique_ptr<RoomTrait> > &traits() const;
		bool solid() const;
	private:
		GridLevel *l() const;
};



GridLevel::GridLevel(const std::string &name, int width, int height) :
	Level(name),
	mWidth(width),
	mHeight(height) {
	mRooms = new RoomData*[width * height];
	std::fill(mRooms, mRooms + (width * height), nullptr);
	for (int i = 0; i < Wall::SideCount; i++) {
		mDefaultRoom.mWalls[i] = &mDefaultWalls[i];
	}

}

GridLevel::~GridLevel() {
	delete[] mRooms;
}

Room GridLevel::room(int localX, int localY) {
	return GridLevelRoom(localX, localY, this);
}

GridLevel::RoomData *&GridLevel::roomData(int x, int y) {
	return mRooms[x + y * mWidth];
}

GridLevelRoom::GridLevelRoom(int x, int y, GridLevel *gridLevel) :
	Room(x, y, gridLevel)
{

}

GridLevelRoom::~GridLevelRoom() {

}

std::vector<std::pair<Item *, Wall::Side> > GridLevelRoom::items() const {
	GridLevel::RoomData *roomData = l()->roomData(mX, mY);
	if (!roomData) {
		roomData = &(l()->mDefaultRoom);
	}

	std::vector<std::pair<Item *, Wall::Side>> ret;
	for (int i = 0; i < Wall::SideCount; i++) {
		GridLevel::WallData *wallData = roomData->mWalls[i];
		if (wallData) {
			for (auto &item : wallData->mItems) {
				ret.push_back(std::pair<Item*, Wall::Side>(item.get(), (Wall::Side)i));
			}
		}
	}
	return ret;
}

std::vector<std::shared_ptr<Character> > GridLevelRoom::characters() const {
	auto range = l()->mCharacters.equal_range(Position(mX, mY));
	std::vector<std::shared_ptr<Character> > ret;
	for (auto it = range.first; it != range.second; ++it) {
		ret.emplace_back(it->second);
	}

	return ret;
}

Wall GridLevelRoom::wall(Wall::Side side) const {
	return GridLevelWall(side, this, l()->roomData(mX, mY)->mWalls[side]);
}

const std::vector<std::unique_ptr<RoomTrait>> &GridLevelRoom::traits() const {
	GridLevel::RoomData *roomData = l()->roomData(mX, mY);
	if (!roomData) roomData = &(l()->mDefaultRoom);
	return roomData->mTraits;
}

bool GridLevelRoom::solid() const {
	GridLevel::RoomData *roomData = l()->roomData(mX, mY);
	if (!roomData) roomData = &(l()->mDefaultRoom);
	return roomData->mSolid;
}

GridLevel *GridLevelRoom::l() const {
	return static_cast<GridLevel*>(mLevel);
}


GridLevelWall::GridLevelWall(Side side, const GridLevelRoom *room, GridLevel::WallData *wallData) :
	Wall(side, room, wallData) {

}

GridLevelWall::~GridLevelWall() {

}

const std::vector<std::unique_ptr<Item> > &GridLevelWall::items() const {
	assert(!noWall());
	return wallData()->mItems;
}

bool GridLevelWall::noWall() const {
	return wallData() == nullptr;
}

