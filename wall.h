#ifndef WALL_H
#define WALL_H
#include "item.h"
#include "character.h"
#include <cassert>

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
		Wall(Side side, const Room *room);
		Wall(Side side, const Room *room, void *d);
		virtual ~Wall();
		const Room *room() const;
		virtual const std::vector<std::unique_ptr<Item> > &items() const { assert(0); return *(std::vector<std::unique_ptr<Item> >*)(0);}
		virtual bool noWall() { assert(0); return false;}
		virtual const std::string &looks() const { assert(0); return *(std::string*)0; }
	protected:
		const Room *mRoom;
		Side mSide;
		void *mData;
};

#endif // WALL_H
