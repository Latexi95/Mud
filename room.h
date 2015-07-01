#ifndef ROOM_H
#define ROOM_H
#include "wall.h"
#include <cassert>

class Level;
class RoomTrait;
class Room {
	public:
		enum Flags {
			Solid = 1
		};

		Room(int x, int y, Level *level);
		virtual ~Room();
		virtual std::vector<std::pair<Item *, Wall::Side>> items() const { assert(0); return std::vector<std::pair<Item *, Wall::Side>>(); }
		virtual std::vector<std::shared_ptr<Character>> characters() const { assert(0); return std::vector<std::shared_ptr<Character>>(); }
		virtual Wall wall(Wall::Side side) const { assert(0); return Wall(Wall::SideCount, 0);}
		virtual const std::vector<std::unique_ptr<RoomTrait> > &traits() const {assert(0); return *(std::vector<std::unique_ptr<RoomTrait> >*)(0);}
		virtual bool solid() const { assert(0); return true; }

		int x() const;
		int y() const;
		Level *level() const { return mLevel; }
	protected:
		Level *mLevel;
		int mX;
		int mY;
		void *mData;
};

#endif // ROOM_H
