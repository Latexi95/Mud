#ifndef ROOM_H
#define ROOM_H
#include <cassert>
#include <array>
#include "wall.h"
#include "traits/roomtrait.h"
#include "defines.h"

#include "jsonserializable.h"

class Level;
struct RoomData;
class Room {
    template<typename T>
    friend class Json::Serializer;
    friend class Level;
public:
    Room();
    Room(const std::string &id);
    Room(const std::string &id, const std::string &name, Level *level);
    ~Room();
    std::vector<std::pair<Item *, Direction>> items() const;
    const std::vector<std::shared_ptr<Character>> &characters() const;
    void addCharacter(const std::shared_ptr<Character> &c);
    void removeCharacter(const std::shared_ptr<Character> &c);

    const Wall &wall(Direction side) const;
    Wall &wall(Direction side);
    const std::array<Wall, (int)Direction::Count> &walls() const;

    const std::vector<std::unique_ptr<RoomTrait>> &traits() const;
    void addTrait(std::unique_ptr<RoomTrait> &&r);

    const std::unordered_map<std::string, std::string> &lookMap() const;
    Level *level() const { return mLevel; }

    void setWall(Direction dir, const Wall &w);
    std::string id() const;
    void setId(const std::string &id);

    void sendEventToCharacters(Event *e);

    RoomExit *exit(Direction side);

protected:
    std::string mName;
    std::string mId;
    Level *mLevel;

    std::unordered_map<std::string, std::string> mLookMap;
    std::array<Wall, (int)Direction::Count> mWalls;
    std::vector<std::shared_ptr<Character>> mCharacters;

    std::vector<std::unique_ptr<RoomTrait>> mTraits;
};

namespace Json {

template<>
struct Serializer<Room> {
    static Value serialize(const Room &r);
    static void deserialize(const Value &v, Room &r);
};
}


#endif // ROOM_H
