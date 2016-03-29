#ifndef TRAITPROPERTY_H
#define TRAITPROPERTY_H
#include <type_traits>
#include <unordered_map>
class TraitPropertyBase;
class TraitPropertyValue;

typedef std::unordered_map<TraitPropertyBase*, TraitPropertyValue> TraitPropertyValueMap;


class TraitPropertyBase {
public:
    ~TraitPropertyBase() {}

    virtual void destructValue(TraitPropertyValue *v) = 0;
};

template <typename T>
class TraitProperty;

class TraitPropertyValue
{
public:
    TraitPropertyValue(const TraitPropertyValue &v) = default;

    template<typename T>
    friend class TraitProperty;
private:
    TraitPropertyValue() {}

    alignas(alignof(std::string)) std::uint8_t mData[sizeof(std::string)];
};

namespace {

template <typename T>
struct OperatorHelper {
    static const bool fits = sizeof(T) <= sizeof(std::string) && alignof(T) <= alignof(std::string);
    static std::enable_if<fits, void>::type copyConstruct(TraitPropertyValue &p, const T &v) {
        new (static_cast<T*>(p.mData)) T(v);
    }
    static std::enable_if<!fits, void>::type copyConstruct(TraitPropertyValue &p, const T &v) {
        *static_cast<T**>(p.mData) = new T(v);
    }
    static std::enable_if<fits, void>::type moveConstruct(TraitPropertyValue &p, T &&v) {
        new (static_cast<T*>(p.mData)) T(std::move(v));
    }
    static std::enable_if<!fits, void>::type moveConstruct(TraitPropertyValue &p, const T &v) {
        *static_cast<T**>(p.mData) = new T(std::move(v));
    }

    static std::enable_if<fits, void>::type destruct(TraitPropertyValue &p) {
        static_cast<T*>(p.mData)->~T();
    }
    static std::enable_if<!fits, void>::type destruct(TraitPropertyValue &p) {
        delete *static_cast<T**>(p.mData);
    }

    static std::enable_if<fits, void>::type copyAssign(TraitPropertyValue &p, const T &v) {
        *static_cast<T*>(p.mData) = v;
    }
    static std::enable_if<!fits, void>::type copyAssign(TraitPropertyValue &p, const T &v) {
        **static_cast<T**>(p.mData) = v;
    }
    static std::enable_if<fits, void>::type moveAssign(TraitPropertyValue &p, T &&v) {
        *static_cast<T*>(p.mData) = v;
    }
    static std::enable_if<!fits, void>::type moveAssign(TraitPropertyValue &p, T &&v) {
        **static_cast<T**>(p.mData) = v;
    }

    static std::enable_if<fits, T&>::type load(TraitPropertyValue &p) {
        return *static_cast<T*>(p.mData);
    }
    static std::enable_if<!fits, T&>::type load(TraitPropertyValue &p) {
        return **static_cast<T**>(p.mData);
    }
};

template <typename T>
struct DefaultConstruct {
    static T construct() { return T(); }
};

}



template <typename T, typename DEFAULT_CONSTRUCT = DefaultConstruct<T> >
class TraitProperty : public TraitPropertyBase {
public:
    TraitProperty(const std::string &i);
    ~TraitProperty();

    void store(TraitPropertyValueMap &m, const T &v) const {
        auto it = m.find(this);
        if (it != m.end()) {
            OperatorHelper<T>::copyAssign(it->second, v);
        }
        else {
            TraitPropertyValue p;
            OperatorHelper<T>::copyConstruct(p, v);
            m.emplace(this, p);
        }
    }

    void store(TraitPropertyValueMap &m, T &&v) const {
        auto it = m.find(this);
        if (it != m.end()) {
            OperatorHelper<T>::copyAssign(it->second, std::move(v));
        }
        else {
            TraitPropertyValue p;
            OperatorHelper<T>::moveConstruct(p, std::move(v));
            m.emplace(this, p);
        }
    }

    bool contains(const TraitPropertyValueMap &m) const {
        return m.find(this) != m.end();
    }

    T &load(TraitPropertyValueMap &m) {
        auto it = m.find(this);
        if (it == m.end()) {
            auto result = m.emplace(this, DEFAULT_CONSTRUCT::construct());
            it = result.first;
        }
        return OperatorHelper<T>::load(it->second);
    }

    T take(TraitPropertyValueMap &m) {
        auto it = m.find(this);
        if (it == m.end()) {
            return DEFAULT_CONSTRUCT::construct();
        }

        T val = std::move(OperatorHelper<T>::load(it->second));
        OperatorHelper<T>::destruct(it->second);
        m.erase(it);
        return std::move(val);
    }

    const std::string &id() const { return mId; }
private:
    std::string mId;
};


#endif // TRAITPROPERTY_H
