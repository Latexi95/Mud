#ifndef ROOM_H
#define ROOM_H
#include "wall.h"
class Level;
class Room {
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

		Room(Level *level);
		virtual ~Room();
		virtual std::vector<RHandle<Item>> items() const = 0;
		virtual std::vector<RHandle<Character>> characters() const = 0;
		virtual Wall &wall(Side side) = 0;


		int x() const;
		int y() const;
		Level *level() const { return mLevel; }
	protected:
		Level *mLevel;
		int mX;
		int mY;
};

#endif // ROOM_H
