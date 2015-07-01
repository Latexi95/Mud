#include "wall.h"

Wall::Wall(Side side, const Room *room) :
	mRoom(room),
	mSide(side),
	mData(0)
{
}

Wall::Wall(Wall::Side side, const Room *room, void *d) :
	mRoom(room),
	mSide(side),
	mData(d)
{

}

Wall::~Wall()
{
}

const Room *Wall::room() const {
	return mRoom;
}
