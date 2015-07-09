#include "wall.h"

Wall::Wall(Side side, const Room *room, std::list<WallData>::iterator wallData) :
    mRoom(room),
    mSide(side),
    mData(0)
{
}


Wall::~Wall()
{
}

const Room *Wall::room() const {
    return mRoom;
}

const std::vector<std::unique_ptr<Item> > &Wall::items() const {
    return mData->mItems;
}

bool Wall::solid() {
    return mData->mSolid;
}

const std::string &Wall::looks() const {
    return mData->mLooks;
}
