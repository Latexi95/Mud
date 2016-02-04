#ifndef LEVEL_H
#define LEVEL_H
#include <vector>
#include <string>
#include <cstdint>
#include "item.h"
#include "room.h"
#include "jsonserializable.h"
#include "position.h"
#include "leveleventqueue.h"
#include "room.h"

class Item;
class Character;
class LevelEventQueue;
class LevelService;
class Level {
public:
    friend class LevelService;

    template <typename T>
    friend struct Json::Serializer;

    Level();
    Level(const std::string &id);
    ~Level();

    const std::string &name() const;
    void setName(const std::string &value);

    const std::string &id() const;

    Json::Value serialize() const;
    void deserialize(const Json::Value &val);

    Room *roomById(const std::string &id) const;
    bool hasRoomById(const std::string &id) const;
    void addRoom(std::unique_ptr<Room> &&room);

    Room *defaultRoom() const;
    const std::vector<std::string> &roomIds() const;

    const std::shared_ptr<LevelEventQueue> &eventQueue() const { return mEventQueue; }

    void sendEventToCharacters(Event *e);

    template <typename FUNC>
    void forEachCharacter(FUNC &&f);
protected:
    bool resolveRoomExits();

    std::string mId;
    std::string mName;
    std::string mDefaultRoomId;
    std::unordered_map<std::string, std::unique_ptr<Room>> mRooms;
    std::vector<std::string> mRoomIds;
    std::shared_ptr<LevelEventQueue> mEventQueue;
};

template <typename FUNC>
void Level::forEachCharacter(FUNC &&f)
{
    for (auto &rPair : mRooms) {
        for (auto &characterPtr : rPair.second->characters()) {
            f(*characterPtr);
        }
    }
}

namespace Json {
template <>
struct Serializer<Level> {
    static Value serialize(const Level &l);
    static void deserialize(const Value &v, Level &l);
};
}

#endif // LEVEL_H
