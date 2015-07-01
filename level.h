#ifndef LEVEL_H
#define LEVEL_H
#include <vector>
#include <string>
#include "item.h"
#include "room.h"
#include <cstdint>
class Item;
class Character;
class Level {
	public:
		Level(const std::string &name);
		virtual ~Level();

		std::string name() const;
		void setName(const std::string &value);
		std::vector<Character*> character();
		virtual Room room(int localX, int localY) = 0;
		int64_t globalX() const { return mGlobalX; }
		int64_t globalY() const { return mGlobalY; }

	protected:
		int64_t mGlobalX;
		int64_t mGlobalY;
		std::string mName;
};

#endif // LEVEL_H
