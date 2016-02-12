#include "roomtrait.h"

RoomTrait::RoomTrait() {
}

Json::Value Json::Serializer<RoomTrait>::serialize(const RoomTrait &r)
{
    return "roomTrait";
}

void Json::Serializer<RoomTrait>::deserialize(const Json::Value &jsonVal, RoomTrait &v)
{
}
