#include "room.h"
#include "level.h"

Room::Room(int x, int y, Level *level) :
    mLevel(level),
    mX(x),
    mY(y)
{
    mData = level->roomData(x, y);
}

Room::~Room() {

}

std::vector<std::pair<Item *, Direction> > Room::items() const {
    std::vector<std::pair<Item*, Direction> > ret;
    for (int side = 0; side < Direction::DirectionCount; ++side) {
        WallData *wd = &(*mData->mWalls[side]);
        for (const auto &itemPtr : wd->mItems) {
            ret.emplace_back(itemPtr.get(), (Direction)side);
        }
    }
    return ret;
}

std::vector<std::shared_ptr<Character> > Room::characters() const {
    auto charMapRange = mLevel->mCharacters.equal_range(Position(mX, mY));

    std::vector<std::shared_ptr<Character> > ret;

    for (auto it = charMapRange.first; it != charMapRange.second; ++it) {
        ret.emplace_back(it->second);
    }
    return ret;
}

Wall Room::wall(Direction side) const {
    return Wall(side, this, mData->mWalls[side]);
}

const std::vector<std::unique_ptr<RoomTrait> > &Room::traits() const {
    return mData->mTraits;
}

bool Room::solid() const {
    return mData->mSolid;
}

const std::unordered_map<std::string, std::string> &Room::lookMap() const {
    return mData->mLooks;
}
