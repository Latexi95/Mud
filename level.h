#ifndef LEVEL_H
#define LEVEL_H
#include <vector>
#include <string>
#include "item.h"
#include "room.h"
#include <cstdint>
#include "jsonserializable.h"
#include "position.h"
#include <list>
#include "leveleventqueue.h"

class Item;
class Character;
class LevelEventQueue;
class Level : public JsonSerializable {
public:
    friend class Room;
    friend class Wall;

    Level(const std::string &id);
    ~Level();



    const std::string &name() const;
    void setName(const std::string &value);

    const std::string &id() const;

    Room room(int localX, int localY);
    Json::Value serialize() const;
    void deserialize(const Json::Value &val);

    RoomData *roomData(int x, int y) const;

    void addCharacter(const std::shared_ptr<Character> &c);
    void removeCharacter(const std::shared_ptr<Character> &c);

    void moveCharacter(const std::shared_ptr<Character> &c, const Position &pos);

    std::shared_ptr<LevelEventQueue> eventQueue() const { return mEventQueue; }

    void sendEventToCharacters(Event *e);
protected:
    void init();
    std::string mId;
    std::string mName;
    unsigned mWidth, mHeight;
    double mRoomSize;
    std::list<RoomData>::iterator *mRoomData;
    std::list<RoomData> mUniqueRoomData;
    std::list<WallData> mUniqueWallData;
    std::unordered_multimap<Position, std::shared_ptr<Character> > mCharacters;
    std::shared_ptr<LevelEventQueue> mEventQueue;
};

#endif // LEVEL_H
