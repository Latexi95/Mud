#ifndef BOX_H
#define BOX_H
#include "util/jsonserializable.h"
#include "util/textutils.h"
template <typename T>
struct Box
{
    Box() : x(0), y(0), z(0) {}
    Box(T x, T y, T z) : x(x), y(y), z(z) {}
    Box(const Box<T> &b) : x(b.x), y(b.y), z(b.z) {}

    Box<T> &operator= (const Box<T> &b) { x = b.x; y = b.y; z = b.z; return *this; }
    bool operator == (const Box<T> &b) const { return x = b.x && y = b.y && z == b.z; }
    bool operator != (const Box<T> &b) const { return x != b.x || y != b.y || z != b.z; }

    T width() const { return x; }
    T height() const { return y; }
    T depth() const { return z; }

    T volume() const { return x * y * z; }
    Box<T> expanded(T xe, T ye, T ze) { return Box<T>(x + xe, y + ye, z + ze); }
    Box<T> expanded(const Box<T> &b) { return Box<T>(x + b.x, y + b.y, z + b.z); }

    std::string toString() const;
    static Box<T> fromString(const std::string &string);

    T x, y, z;
};
template <typename T>
std::string Box<T>::toString() const
{
    return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
}

template <typename T>
Box<T> Box<T>::fromString(const std::string &string)
{
    std::vector<std::string> parts = text::split(string,',');
    if (parts.size() != 3) {
        throw SerializationException("Expecting 3 values separated with ','");
    }

    for (std::string &p : parts) {
        text::strip(p);
    }
    try {
        return Box<T>(lexical_cast<T>(parts[0]), boost::lexical_cast<T>(parts[1]), boost::lexical_cast<T>(parts[2]));
    }
    catch (const boost::bad_lexical_cast &c) {
        throw SerializationException("Can't convert to a number");
    }
}

namespace Json {

template <typename T>
struct Serializer<Box<T> > {
    static Json::Value serialize(const Box<T> &b) {
        Json::Value value(Json::objectValue);
        value["x"] = b.x;
        value["y"] = b.y;
        value["z"] = b.z;
        return value;
    }
    static void deserialize(const Json::Value &val, Box<T> &b) {
        if (!val.isObject()) throw SerializationException("deserialize<Box> expecting an json object");
        Json::deserialize(val.get("x", Json::Value(b.x)), b.x);
        Json::deserialize(val.get("y", Json::Value(b.y)), b.y);
        Json::deserialize(val.get("z", Json::Value(b.z)), b.z);
    }
};

}

#endif // BOX_H
