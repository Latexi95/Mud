#ifndef WALL_H
#define WALL_H
#include "item.h"
#include "character.h"
#include <cassert>
#include <list>
#include "enums.h"
struct WallData;



class Room;
class Wall {
public:

    Wall(Direction side, const Room *room, std::list<WallData>::iterator wallData);
    ~Wall();
    const Room *room() const;
    const std::vector<std::unique_ptr<Item> > &items() const;
    bool solid();
    const std::string &looks() const;
protected:
    const Room *mRoom;
    Direction mSide;
    std::list<WallData>::iterator mData;
};

struct WallData {
    std::string mLooks;
    std::vector<std::unique_ptr<Item> > mItems;
    bool mSolid;
    unsigned mRefCount;
};

#endif // WALL_H
