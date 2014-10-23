#include "level.h"

Level::Level(const std::string &name):
	mName(name)
{
}

Level::~Level() {
}
std::string Level::name() const {
	return mName;
}

void Level::setName(const std::string &value) {
	mName = value;
}

