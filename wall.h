#ifndef WALL_H
#define WALL_H
#include "item.h"
#include "character.h"
#include <cassert>
#include <list>

struct WallData;

class Room;
class Wall {
	public:
		enum Side {
			North,
			South,
			East,
			West,
			Up,
			Down,
			SideCount
		};
		Wall(Side side, const Room *room, std::list<WallData>::iterator wallData);
		~Wall();
		const Room *room() const;
		const std::vector<std::unique_ptr<Item> > &items() const;
		bool solid();
		const std::string &looks() const;
	protected:
		const Room *mRoom;
		Side mSide;
		std::list<WallData>::iterator mData;
};

struct WallData {
	std::string mLooks;
	std::vector<std::unique_ptr<Item> > mItems;
	bool mSolid;
	unsigned mRefCount;
};

#endif // WALL_H
