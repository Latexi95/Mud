#ifndef ROOM_H
#define ROOM_H
#include "item.h"
#include "resource.h"
class Level;
class Room {
	public:
		Room(Level *level);
		virtual ~Room();
		virtual std::vector<RHandle<Item>> items() const = 0;
		int x() const;
		int y() const;
		Level *level() const { return mLevel; }
	protected:
		Level *mLevel;
		int mX;
		int mY;
};

#endif // ROOM_H
