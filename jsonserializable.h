#ifndef JSONSERIALIZABLE_H
#define JSONSERIALIZABLE_H
#include <json/value.h>
#include <memory>
#include <utility>
#include <type_traits>
#include "common.h"

class JsonSerializable {
public:
    JsonSerializable();
    virtual ~JsonSerializable();
    virtual Json::Value serialize() const = 0;
    virtual void deserialize(const Json::Value &val) = 0;
protected:

};

class SerialiazationException : public std::exception {
public:
    SerialiazationException(const std::string &txt) : msg(txt) {}
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
        return t->serialize();
    }
    static void deserialize(const Json::Value &jsonVal, std::unique_ptr<T> &t) {
        t->deserialize(jsonVal);
    }
};


template<>
struct Serializer<std::string> {
    static Json::Value serialize(const std::string &value) {
        return value;
    }

    static void deserialize(const Value &jsonVal, std::string &s) {
        if (!jsonVal.isString()) throw SerialiazationException("deserialize<std::string>: Expecting a string");
        s = jsonVal.asString();
    }
};

template<>
struct Serializer<int> {
    static Json::Value serialize(int value) {
        return value;
    }

    static void deserialize(const Value &jsonVal, int &s) {
        if (!jsonVal.isInt()) throw SerialiazationException("deserialize<int>: Expecting an integer");
        s = jsonVal.asInt();
    }
};


template<>
struct Serializer<double> {
    static Json::Value serialize(double value) {
        return value;
    }

    static void deserialize(const Value &jsonVal, double &s) {
        if (!jsonVal.isDouble()) throw SerialiazationException("deserialize<double>: Expecting a double");
        s = jsonVal.asDouble();
    }
};

template<typename T>
struct Serializer<std::vector<T> > {
    static Json::Value serialize(const std::vector<T> &vec) {
        Json::Value val(Json::arrayValue);
        for (auto &t : vec) {
            val.append(Json::serialize(t));
        }
        return val;
    }

    static void deserialize(const Value &val, std::vector<T> &vec) {
        if (!val.isArray()) throw SerialiazationException("Expecting an array type to deserialize to std::vector<T>");
        for (const Json::Value &v : val) {
            T t;
            Json::deserialize(v, t);
            vec.emplace_back(std::move(t));
        }
    }
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
