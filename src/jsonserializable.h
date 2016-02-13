#ifndef JSONSERIALIZABLE_H
#define JSONSERIALIZABLE_H
#include <json/value.h>
#include <memory>
#include <utility>
#include <type_traits>
#include <unordered_map>
#include "enums.h"
#include "common.h"

class SerializationException : public std::exception {
public:
    SerializationException(const std::string &txt) : msg(txt) {}
    virtual const char* what() const noexcept { return msg.c_str(); }
    const std::string &message() const { return msg; }
private:
    std::string msg;
};

namespace Json {

template<typename T>
Json::Value serialize(T &&t);

template<typename T>
void deserialize(const Value &jsonVal, T &v);

template <typename T>
struct Serializer {
    static Json::Value serialize(const T &t) {
        return t.serialize();
    }
    static void deserialize(const Json::Value &jsonVal, T &t) {
        t.deserialize(jsonVal);
    }
};


template <typename T>
struct Serializer<std::unique_ptr<T> > {
    static Json::Value serialize(const std::unique_ptr<T> &t) {
        return Json::serialize(*t);
    }
    static void deserialize(const Json::Value &jsonVal, std::unique_ptr<T> &t) {
        if (!t) t = std::unique_ptr<T>(new T());
        Json::deserialize(jsonVal, *t);
    }
};
template <typename T>
struct Serializer<std::shared_ptr<T> > {
    static Json::Value serialize(const std::shared_ptr<T> &t) {
        return Json::serialize(*t);
    }
    static void deserialize(const Json::Value &jsonVal, std::shared_ptr<T> &t) {
        if (!t) t = std::make_shared<T>();
        Json::deserialize(jsonVal, *t);
    }
};


template<>
struct Serializer<std::string> {
    static Json::Value serialize(const std::string &value);
    static void deserialize(const Value &jsonVal, std::string &s);
};

template<>
struct Serializer<bool> {
    static Json::Value serialize(bool b);
    static void deserialize(const Value &v, bool &b);
};

template<>
struct Serializer<int> {
    static Json::Value serialize(int value);
    static void deserialize(const Value &jsonVal, int &s);
};


template<>
struct Serializer<double> {
    static Json::Value serialize(double value);
    static void deserialize(const Value &jsonVal, double &s);
};

template<>
struct Serializer<float> {
    static Json::Value serialize(float value);
    static void deserialize(const Value &jsonVal, float &s);
};


template<typename T>
struct Serializer<std::vector<T> > {
    static Json::Value serialize(const std::vector<T> &vec) {
        if (vec.empty()) return nullValue;

        Json::Value val(Json::arrayValue);
        for (auto &t : vec) {
            val.append(Json::serialize(t));
        }
        return val;
    }

    static void deserialize(const Value &val, std::vector<T> &vec) {
        if (val.isNull()) return;
        if (!val.isArray()) throw SerializationException("Expecting an array type to deserialize to std::vector<T>");
        for (const Json::Value &v : val) {
            T t;
            Json::deserialize(v, t);
            vec.emplace_back(std::move(t));
        }
    }
};

template<typename T>
struct Serializer<std::unordered_map<std::string, T> > {
    static Json::Value serialize(const std::unordered_map<std::string, T> &m) {
        Json::Value obj(objectValue);
        for (const auto &p : m) {
            obj[p.first] = Json::serialize(p.second);
        }
        return obj;
    }
    static void deserialize(const Value &v, std::unordered_map<std::string, T> &m) {
        if (v.isNull()) return;
        if (!v.isObject()) throw SerializationException("Expecting an object type to deserialize to std::unordered_map<std::string, T>");

        for (auto it = v.begin(); it != v.end(); ++it) {
            Json::deserialize(*it, m[it.name()]);
        }
    }
};

template<>
struct Serializer<Direction> {
    static Json::Value serialize(Direction d);
    static void deserialize(const Json::Value &v, Direction &d);
};


template<typename T>
Json::Value serialize(T &&t) {
    typedef typename std::remove_cv<typename std::remove_reference<T>::type>::type base_type;
    return Serializer<base_type>::serialize(t);
}

template<typename T>
void deserialize(const Value &jsonVal, T &v) {
    Serializer<typename std::remove_cv<typename std::remove_reference<T>::type>::type >::deserialize(jsonVal, v);
}

}

#endif // JSONSERIALIZABLE_H
