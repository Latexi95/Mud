#ifndef ROOMEXIT_H
#define ROOMEXIT_H
#include <string>
#include <memory>
#include "defines.h"
#include "jsonserializable.h"

class Room;
class Level;

class RoomExit
{
    template <typename T>
    friend struct Json::Serializer;
public:
    RoomExit();
    RoomExit(const std::string &name, Room *targetRoom);
    ~RoomExit();

    std::string name() const;
    void setName(const std::string &name);

    Room *targetRoom() const;
    void setTargetRoom(Room *targetRoom);

    float travelDistance() const;
    void setTravelDistance(float travelDistance);
    std::string targetRoomID() const;

    bool resolveLink(Level *roomLevel);

private:
    std::string mName;
    float mTravelDistance;
    std::string mTargetRoomID;
    Room *mTargetRoom;
};

namespace Json {
template<>
struct Serializer<RoomExit> {
    static Json::Value serialize(const RoomExit &b);
    static void deserialize(const Json::Value &val, RoomExit &b);
};

}

#endif // ROOMEXIT_H
