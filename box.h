#ifndef BOX_H
#define BOX_H
#include "jsonserializable.h"
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

    T x, y, z;
};
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
        if (!val.isObject()) throw SerialiazationException("deserialize<Box> expecting an json object");
        Json::deserialize(val.get("x", Json::Value(b.x)), b.x);
        Json::deserialize(val.get("y", Json::Value(b.y)), b.y);
        Json::deserialize(val.get("z", Json::Value(b.z)), b.z);
    }
};

}

#endif // BOX_H
