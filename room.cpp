#include "room.h"

Room::Room(const std::string &name):
	mName(name)
{
}

Room::~Room() {
}
std::string Room::name() const {
	return mName;
}

void Room::setName(const std::string &value) {
	mName = value;
}

