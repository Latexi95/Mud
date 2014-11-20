#include "wall.h"

Wall::Wall(Room *room) :
	mRoom(room)
{
}

Room *Wall::room() const {
	return mRoom;
}
