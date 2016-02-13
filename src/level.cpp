#include "level.h"
#include <unordered_set>
#include <boost/lexical_cast.hpp>
#include "item.h"
#include "room.h"
#include <boost/thread.hpp>

thread_local Level *LEVEL;

Level::Level() :
    mId(),
    mEventQueue(this)
{
}

Level::Level(const std::string &id) :
    mId(id),
    mEventQueue(this)
{
}

Level::~Level() {

}

const std::string &Level::name() const {
    return mName;
}

void Level::setName(const std::string &value) {
    mName = value;
}

const std::string &Level::id() const {
    return mId;
}


Room *Level::roomById(const std::string &id) const
{
    auto it = mRooms.find(id);
    assert(it != mRooms.end());
    return it->second.get();
}

bool Level::hasRoomById(const std::string &id) const
{
    auto it = mRooms.find(id);
    return it != mRooms.end();
}

void Level::addRoom(std::unique_ptr<Room> &&room)
{
    room->mLevel = this;
    mRooms[room->id()] = std::move(room);
}

Room *Level::defaultRoom() const
{
    return roomById(mDefaultRoomId);
}

const std::vector<std::string> &Level::roomIds() const
{
    return mRoomIds;
}

void Level::sendEventToCharacters(Event *e) {
    for (auto &room : mRooms) {
        room.second->sendEventToCharacters(e);
    }
}

bool Level::isAccessSafe() const
{
    return current() == (Level*)-1 || current() == this;
}

Level *Level::current()
{
    return LEVEL;
}

Level *Level::setCurrent(Level *l)
{
    LEVEL = l;
}

bool Level::resolveRoomExits()
{
    bool success = true;
    for (auto &room : mRooms) {
        for (const Wall &wall : room.second->walls()) {
            if (RoomExit *exit = wall.exit()) {
                success &= exit->resolveLink(this);
            }
        }
    }
    return success;
}

Json::Value Json::Serializer<Level>::serialize(const Level &l)
{
    Json::Value root(Json::objectValue);
    root["name"] = l.mName;
    root["default_room"] = l.mDefaultRoomId;

    root["rooms"] = Json::serialize(l.mRoomIds);
    return root;
}

void Json::Serializer<Level>::deserialize(const Json::Value &v, Level &l)
{
    Json::deserialize(v["name"], l.mName);
    Json::deserialize(v["default_room"], l.mDefaultRoomId);

    Json::deserialize(v["rooms"], l.mRoomIds);
}

