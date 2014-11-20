#ifndef WALL_H
#define WALL_H
#include "item.h"
#include "character.h"
#include "resource.h"
class Room;
class Wall {
	public:
		Wall(Room *room);
		virtual ~Wall();
		Room *room() const;
		virtual std::vector<RHandle<Item> > items() const = 0;
		virtual bool noWall() const = 0;
	protected:
		Room *mRoom;
};

#endif // WALL_H
