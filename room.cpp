#include "room.h"

Room::Room(int x, int y, Level *level) :
	mLevel(level),
	mX(x),
	mY(y),
	mData(0)
{
}

Room::~Room() {

}
