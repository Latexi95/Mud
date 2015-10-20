#ifndef ROOM_H
#define ROOM_H
#include "wall.h"
#include <cassert>
#include "traits/roomtrait.h"
#include <array>

class Level;
struct RoomData;
class Room {
public:
    enum Flags {
        Solid = 1
    };

    Room(int x, int y, Level *level);
    ~Room();
    std::vector<std::pair<Item *, Direction>> items() const;
    std::vector<std::shared_ptr<Character>> characters() const;
    Wall wall(Direction side) const;
    const std::vector<std::unique_ptr<RoomTrait> > &traits() const;
    bool solid() const;

    const std::unordered_map<std::string, std::string> &lookMap() const;

    int x() const;
    int y() const;
    Level *level() const { return mLevel; }
protected:
    Level *mLevel;
    int mX;
    int mY;
    RoomData *mData;
};

struct RoomData {
    RoomData() : mWalls(), mSolid(true), mRefCount(0) {}
    std::array<std::list<WallData>::iterator, (unsigned int)Direction::DirectionCount> mWalls;
    std::vector<std::unique_ptr<RoomTrait>> mTraits;
    std::unordered_map<std::string, std::string> mLooks;
    bool mSolid;
    unsigned mRefCount;
};

#endif // ROOM_H
