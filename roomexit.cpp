#include "roomexit.h"
#include "room.h"
#include "level.h"
#include "levelservice.h"

RoomExit::RoomExit() :
    mTargetRoom(nullptr)
{
}

RoomExit::RoomExit(const std::string &name, Room *targetRoom) :
    mName(name),
    mTargetRoom(targetRoom)
{
}

RoomExit::~RoomExit()
{
}

std::string RoomExit::name() const
{
    return mName;
}

void RoomExit::setName(const std::string &name)
{
    mName = name;
}

Room *RoomExit::targetRoom() const
{
    return mTargetRoom;
}

void RoomExit::setTargetRoom(Room *targetRoom)
{
    mTargetRoom = targetRoom;
    mTargetRoomID = mTargetRoom->id();
}

float RoomExit::travelDistance() const
{
    return mTravelDistance;
}

void RoomExit::setTravelDistance(float travelDistance)
{
    mTravelDistance = travelDistance;
}

std::string RoomExit::targetRoomID() const
{
    return mTargetRoomID;
}

bool RoomExit::resolveLink(Level *roomLevel)
{
    //Different level?
    auto levelSeparatorIndex = mTargetRoomID.find_last_of('/');
    if (levelSeparatorIndex != std::string::npos) {
        std::string levelId = mTargetRoomID.substr(0, levelSeparatorIndex);
        std::string roomId = mTargetRoomID.substr(levelSeparatorIndex + 1);

        auto level = LS->level(levelId);
        if (!level) {
            std::cerr << "Unresolved room link -> " << mTargetRoomID << std::endl;
            return false;
        }
        Room *r = level->roomById(roomId);
        if (!r) {
            std::cerr << "Unresolved room link -> " << mTargetRoomID << std::endl;
            return false;
        }
        mTargetRoom = r;
    }
    else {
        Room *r = roomLevel->roomById(mTargetRoomID);
        if (!r) {
            std::cerr << "Unresolved room link -> " << mTargetRoomID << std::endl;
            return false;
        }
        setTargetRoom(r);
    }
    return true;
}


namespace Json  {
Json::Value Serializer<RoomExit>::serialize(const RoomExit &b) {
    Json::Value value(Json::objectValue);
    value["name"] = b.mName;
    value["travel_distance"] = b.mTravelDistance;
        value["target_room"] = b.mTargetRoomID;
        return value;
    }
void Serializer<RoomExit>::deserialize(const Json::Value &val, RoomExit &b) {
    if (!val.isObject()) throw SerializationException("deserialize<RoomExit> expecting an json object");
    Json::deserialize(val.get("name", Json::Value(b.mName)), b.mName);
    Json::deserialize(val.get("travel_distance", Json::Value(b.mTravelDistance)), b.mTravelDistance);
    Json::deserialize(val.get("target_room", Json::Value(b.mTargetRoomID)), b.mTargetRoomID);
}
}

