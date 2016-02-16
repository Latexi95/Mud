#ifndef PROPERTIES_H
#define PROPERTIES_H
#include <string>
#include <utility>
#include <functional>
#include "com/messagebuilder.h"
#include <map>

namespace editor {

template <typename T>
struct Context {
    T *mTarget;
    std::string mEditPath;
};

template <typename T>
class Properties
{
public:
    typedef T value_type;

    struct Accessor {
        std::function<bool(T*)> mAvailable;
        std::function<void(T*, const std::string)> mSetter;
        std::function<std::string(T*)> mGetter;
    };

    Properties();

    template <typename GETTER, typename SETTER>
    void addProperty(const std::string &name, GETTER &&getter, SETTER &&setter);
    template <typename AVAILABILITY, typename GETTER, typename SETTER>
    void addProperty(const std::string &name, AVAILABILITY &&availibility, GETTER &&getter, SETTER &&setter);

    void addProperty(const std::string &name, Accessor &&accessor);

    bool hasProperty(const Context<T> &c,const std::string &name);
    void removeProperty(const std::string &name);

    bool set(const Context<T> &c, const std::string &name, const std::string &value);
    std::string get(const Context<T> &c,  const std::string &name);
private:
    typename std::map<std::string, Accessor>::iterator findProperty(const Context<T> &c,  const std::string &name);

    std::map<std::string, Accessor> mProperties;
};

template <typename T>
Properties<T>::Properties()
{

}

template <typename T>
template<typename GETTER, typename SETTER>
void Properties<T>::addProperty(const std::string &name, GETTER &&getter, SETTER &&setter)
{
    Properties<T>::Accessor a = {[](T *){return true;}, setter, getter};
    addProperty(name, std::move(a));
}

template <typename T>
template<typename AVAILABILITY, typename GETTER, typename SETTER>
void Properties<T>::addProperty(const std::string &name,AVAILABILITY &&availability, GETTER &&getter, SETTER &&setter)
{
    Properties<T>::Accessor a = {availability, setter, getter};
    addProperty(name, std::move(a));
}

template <typename T>
void Properties<T>::addProperty(const std::string &name, Accessor &&accessor)
{
    mProperties[name] = std::move(accessor);
}
template <typename T>
bool Properties<T>::hasProperty(const Context<T> &c, const std::string &name)
{
    auto it = mProperties.find(name);
    if (it == mProperties.end()) return false;
    return it->second.mAvailable(c.mTarget);
}
template <typename T>
void Properties<T>::removeProperty(const std::string &name)
{
    auto it = mProperties.find(name);
    if (it != mProperties.end()) {
        mProperties.erase(it);
    }
}
template <typename T>
bool Properties<T>::set(const Context<T> &c, const std::string &name, const std::string &value)
{
    auto it = mProperties.find(name);
    if (it == mProperties.end() || !it->second.mAvailable(c.mTarget)) return false;

    it->second.mSetter(c.mTarget, value);
    return true;
}
template <typename T>
std::string Properties<T>::get(const Context<T> &c, const std::string &name)
{
    auto it = findProperty(c, name);
    if (it == mProperties.end()) return std::string();

    return it->second.mGetter(c.mTarget);
}

template <typename T>
typename std::map<std::string, typename Properties<T>::Accessor>::iterator Properties<T>::findProperty(const Context<T> &c, const std::string &name)
{
    auto it = mProperties.find(c.mEditPath + name);
    if (it == mProperties.end()) {
        it = mProperties.find(name);
    }

    return it;
}

}
#endif // PROPERTIES_H
