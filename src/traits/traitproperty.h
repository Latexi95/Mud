#ifndef TRAITPROPERTY_H
#define TRAITPROPERTY_H
#include <type_traits>
#include <unordered_map>
#include "util/jsonserializable.h"
class TraitPropertyBase;
class TraitPropertyValue;

typedef std::unordered_map<const TraitPropertyBase*, TraitPropertyValue> TraitPropertyValueMap;

template <typename T>
struct DefaultConstruct {
    static T construct() { return T(); }
};


class TraitPropertyBase {
public:
    TraitPropertyBase(const std::string &id_);
    ~TraitPropertyBase() {}
    virtual void destructValue(TraitPropertyValue &p) = 0;
    virtual Json::Value serialize(const TraitPropertyValueMap &p) = 0;
    virtual void deserialize(TraitPropertyValueMap &p, const Json::Value &val) = 0;
protected:
    std::string mId;
};


/**
  * BUG: Doesn't work if T contains pointers to itself. (TraitPropertyValueMap moves )
*/

template <typename T, typename DEFAULT_CONSTRUCT = DefaultConstruct<T> >
class TraitProperty;

class TraitPropertyValue
{
public:
    TraitPropertyValue(const TraitPropertyValue &v) = default;

    template <typename T, typename DEFAULT_CONSTRUCT>
    friend class TraitProperty;
private:
    TraitPropertyValue() {}

    alignas(alignof(std::string)) std::uint8_t mData[sizeof(std::string)];


    #define FITS (sizeof(T) <= sizeof(void*) && alignof(T) <= alignof(void*))

    template <typename T>
    static typename std::enable_if<FITS, void>::type copyConstruct(TraitPropertyValue &p, const T &v) {
        new (reinterpret_cast<T*>(p.mData)) T(v);
    }
    template <typename T>
    static typename std::enable_if<!FITS, void>::type copyConstruct(TraitPropertyValue &p, const T &v) {
        *reinterpret_cast<T**>(p.mData) = new T(v);
    }
    template <typename T>
    static typename std::enable_if<FITS, void>::type moveConstruct(TraitPropertyValue &p, T &&v) {
        new (p.mData) T(std::move(v));
    }
    template <typename T>
    static typename std::enable_if<!FITS, void>::type moveConstruct(TraitPropertyValue &p, T &&v) {
        *reinterpret_cast<T**>(p.mData) = new T(std::move(v));
    }

    template <typename T>
    static typename std::enable_if<FITS, void>::type destruct(TraitPropertyValue &p) {
        reinterpret_cast<T*>(p.mData)->~T();
    }
    template <typename T>
    static typename std::enable_if<!FITS, void>::type destruct(TraitPropertyValue &p) {
        delete *reinterpret_cast<T**>(p.mData);
    }

    template <typename T>
    static typename std::enable_if<FITS, void>::type copyAssign(TraitPropertyValue &p, const T &v) {
        *reinterpret_cast<T*>(p.mData) = v;
    }
    template <typename T>
    static typename std::enable_if<!FITS, void>::type copyAssign(TraitPropertyValue &p, const T &v) {
        **reinterpret_cast<T**>(p.mData) = v;
    }
    template <typename T>
    static typename std::enable_if<FITS, void>::type moveAssign(TraitPropertyValue &p, T &&v) {
        *reinterpret_cast<T*>(p.mData) = std::move(v);
    }
    template <typename T>
    static typename std::enable_if<!FITS, void>::type moveAssign(TraitPropertyValue &p, T &&v) {
        **reinterpret_cast<T**>(p.mData) = std::move(v);
    }
    template <typename T>
    static typename std::enable_if<FITS, T&>::type load(TraitPropertyValue &p) {
        return *reinterpret_cast<T*>(p.mData);
    }

    template <typename T>
    static typename std::enable_if<!FITS, T&>::type load(TraitPropertyValue &p) {
        return **reinterpret_cast<T**>(p.mData);
    }

    template <typename T>
    static typename std::enable_if<FITS, const T&>::type load(const TraitPropertyValue &p) {
        return *reinterpret_cast<const T*>(p.mData);
    }

    template <typename T>
    static typename std::enable_if<!FITS, const T&>::type load(const TraitPropertyValue &p) {
        return **reinterpret_cast<const T*const*>(p.mData);
    }
    #undef FITS
};




template <typename T, typename DEFAULT_CONSTRUCT>
class TraitProperty : public TraitPropertyBase {
public:
    typedef T value_type;

    TraitProperty(const std::string &id);
    virtual ~TraitProperty();

    virtual void destructValue(TraitPropertyValue &p);
    virtual Json::Value serialize(const TraitPropertyValueMap &p);
    virtual void deserialize(TraitPropertyValueMap &p, const Json::Value &val);

    void store(TraitPropertyValueMap &m, const T &v) const {
        auto it = m.find(this);
        if (it != m.end()) {
            TraitPropertyValue::copyAssign<T>(it->second, v);
        }
        else {
            TraitPropertyValue p;
            TraitPropertyValue::copyConstruct<T>(p, v);
            m.emplace(this, p);
        }
    }

    void store(TraitPropertyValueMap &m, T &&v) const {
        auto it = m.find(this);
        if (it != m.end()) {
            TraitPropertyValue::moveAssign<T>(it->second, std::move(v));
        }
        else {
            TraitPropertyValue p;
            TraitPropertyValue::moveConstruct<T>(p, std::move(v));
            m.emplace(this, p);
        }
    }

    bool contains(const TraitPropertyValueMap &m) const {
        return m.find(this) != m.end();
    }

    T &load(TraitPropertyValueMap &m) {
        auto it = m.find(this);
        if (it == m.end()) {
            TraitPropertyValue p;
            TraitPropertyValue::moveConstruct<T>(p, DEFAULT_CONSTRUCT::construct());
            auto result = m.emplace(this, p);
            it = result.first;
        }
        return TraitPropertyValue::load<T>(it->second);
    }

    T take(TraitPropertyValueMap &m) {
        auto it = m.find(this);
        if (it == m.end()) {
            return DEFAULT_CONSTRUCT::construct();
        }

        T val = std::move(TraitPropertyValue::load<T>(it->second));
        TraitPropertyValue::destruct<T>(it->second);
        m.erase(it);
        return std::move(val);
    }

    void destructValue(TraitPropertyValueMap &m) {
        auto it = m.find(this);
        if (it == m.end()) return;

        destructValue(it->second);
    }

    const std::string &id() const { return mId; }
};

template <typename T, typename DEFAULT_CONSTRUCT>
TraitProperty<T, DEFAULT_CONSTRUCT>::TraitProperty(const std::string &id) :
    TraitPropertyBase(id)
{
}

template <typename T, typename DEFAULT_CONSTRUCT>
TraitProperty<T, DEFAULT_CONSTRUCT>::~TraitProperty()
{
}

template <typename T, typename DEFAULT_CONSTRUCT>
void TraitProperty<T, DEFAULT_CONSTRUCT>::destructValue(TraitPropertyValue &p)
{
    TraitPropertyValue::destruct<T>(p);
}

template <typename T, typename DEFAULT_CONSTRUCT>
Json::Value TraitProperty<T, DEFAULT_CONSTRUCT>::serialize(const TraitPropertyValueMap &p)
{
    auto it = p.find(this);
    if (it == p.end()) {
        return Json::Value(Json::nullValue);
    }

    const T &v = TraitPropertyValue::load<T>(it->second);
    return Json::serialize(v);
}

template <typename T, typename DEFAULT_CONSTRUCT>
void TraitProperty<T, DEFAULT_CONSTRUCT>::deserialize(TraitPropertyValueMap &p, const Json::Value &val)
{
    auto it = p.find(this);
    if (it != p.end()) {
        TraitPropertyValue::destruct<T>(it->second);
        p.erase(it);
    }

    if (val.isNull()) {
        return;
    }

    TraitPropertyValue pval;
    T v = DEFAULT_CONSTRUCT::construct();
    Json::deserialize(val, v);
    TraitPropertyValue::moveConstruct<T>(pval, std::move(v));

    p.emplace(this, pval);
}


#endif // TRAITPROPERTY_H
