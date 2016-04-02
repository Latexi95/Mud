#ifndef ROOMTRAIT_H
#define ROOMTRAIT_H
#include "util/jsonserializable.h"
class RoomTrait {
public:
    RoomTrait();
};

namespace Json {
template<>
struct Serializer<RoomTrait> {
    static Value serialize(const RoomTrait &r);
    static void deserialize(const Value &jsonVal, RoomTrait &v);
};
}


#endif // ROOMTRAIT_H
