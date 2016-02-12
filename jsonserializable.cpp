#include "jsonserializable.h"
#include <cstring>
#include <boost/algorithm/string.hpp>

Json::Value Json::Serializer<Direction>::serialize(Direction d)
{
    return DirectionToString(d);
}

void Json::Serializer<Direction>::deserialize(const Json::Value &v, Direction &d)
{
    if (v.isString()) {
        std::string id = v.asString();
        boost::to_lower(id);

        if (id.size() == 1) {
            for (unsigned dirNum = 0; dirNum < (unsigned)Direction::Count; ++dirNum) {
                if (*id.c_str() == DirectionToChar((Direction)dirNum)) {
                    d = (Direction)dirNum;
                    return;
                }
            }
        }

        for (unsigned dirNum = 0; dirNum < (unsigned)Direction::Count; ++dirNum) {
            if (id == DirectionToString((Direction)dirNum)) {
                d = (Direction)dirNum;
                return;
            }
        }
    }
    else if (v.isNumeric()) {
        int id = v.asInt();
        if (id >= 0 && id < (int)Direction::Count) {
            d = (Direction)id;
            return;
        }
    }
    throw SerializationException("Serializer<Direction>::deserialize: string value isn't valid 'direction'");
}

void Json::Serializer<float>::deserialize(const Json::Value &jsonVal, float &s) {
    if (!jsonVal.isDouble()) throw SerializationException("deserialize<float>: Expecting a float");
    s = jsonVal.asDouble();
}

Json::Value Json::Serializer<float>::serialize(float value) {
    return value;
}

Json::Value Json::Serializer<double>::serialize(double value) {
    return value;
}

void Json::Serializer<double>::deserialize(const Json::Value &jsonVal, double &s) {
    if (!jsonVal.isDouble()) throw SerializationException("deserialize<double>: Expecting a double");
    s = jsonVal.asDouble();
}

Json::Value Json::Serializer<int>::serialize(int value) {
    return value;
}

void Json::Serializer<int>::deserialize(const Json::Value &jsonVal, int &s) {
    if (!jsonVal.isInt()) throw SerializationException("deserialize<int>: Expecting an integer");
    s = jsonVal.asInt();
}

Json::Value Json::Serializer<std::string>::serialize(const std::string &value) {
    return value;
}

void Json::Serializer<std::string>::deserialize(const Json::Value &jsonVal, std::string &s) {
    if (!jsonVal.isString()) throw SerializationException("deserialize<std::string>: Expecting a string");
    s = jsonVal.asString();
}

Json::Value Json::Serializer<bool>::serialize(bool b)
{
    return b;
}

void Json::Serializer<bool>::deserialize(const Json::Value &v, bool &b)
{
    b = v.asBool();
}
