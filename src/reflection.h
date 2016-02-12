#pragma once
#include <typeinfo>
#include <vector>
#include <memory>
#include <sstream>



template <typename T>
struct NumberLimit {
    typedef T ValueType;

    NumberLimit(T minVal, T maxVal) : mMinVal(minVal), mMaxVal(maxVal) {}


    bool operator()(T v) {
        return v >= mMinVal && v <= mMaxVal;
    }

    std::string errorMessage(T v) {
        std::ostringstream s;
        s << "Value (" << v << ") should be between " << mMinVal << " and " << mMaxVal << ".";
        return s.str();
    }

    T mMinVal, mMaxVal;
};

template <typename T>
struct NoLimit {
    typedef T ValueType;
    bool isInLimits(T v) { return true; }

    std::string errorMessage(T v) { return std::string(); }
};


struct ClassProperty {
    ClassProperty(const std::type_info &info, const std::string &name) : mTypeInfo(info), mName(name) {}
    virtual ~ClassProperty() {}
    const std::type_info &mTypeInfo;
    const std::string mName;
};

template <typename T, typename CLASS_T, typename LIMIT>
struct ClassPropertyInstance : public ClassProperty {
    ClassPropertyInstance(const std::string &name, T CLASS_T::* varPtr, LIMIT &&limit) : ClassProperty(typeid(T), name), mVarPtr(varPtr), mLimit(std::move(limit)) {}

    typedef CLASS_T ClassType;
    typedef T ValueType;

    bool isInLimits(const T &v) { return mLimit(v); }
    std::string errorMessage(const T &v) { return mLimit.errorMessage(v); }

    T CLASS_T::* mVarPtr;
    T mDefaultValue;
    LIMIT mLimit;
};

template <typename T, typename CLASS_T>
std::unique_ptr<ClassProperty> makeClassPropertyInstance (T CLASS_T::* varPtr, const std::string &name) {
    return std::unique_ptr<ClassProperty>(new ClassPropertyInstance<T, CLASS_T, NoLimit<T>>(name, varPtr, NoLimit<T>()));
}

template <typename T, typename CLASS_T, typename LIMIT>
std::unique_ptr<ClassPropertyInstance<T, CLASS_T, LIMIT> > makeClassPropertyInstance (T CLASS_T::* varPtr, const std::string &name, LIMIT &&limit) {
    return std::unique_ptr<ClassProperty>(new ClassPropertyInstance<T, CLASS_T, LIMIT>(name, varPtr, std::move(limit)));
}

template <typename T, typename CLASS_T, typename LIMIT, typename GETTER, typename SETTER>
struct ClassPropertyInstanceGetterSetter : public ClassProperty {
    ClassPropertyInstanceGetterSetter(const std::string &name, GETTER getter, SETTER setter, LIMIT &&limit) :
        ClassProperty(typeid(T), name), mLimit(std::move(limit)), mGetter(getter), mSetter(setter) {}

    typedef CLASS_T ClassType;
    typedef T ValueType;
    typedef GETTER GetterFuncType;
    typedef SETTER SetterFuncType;

    bool isInLimits(const T &v) { return mLimit(v); }
    std::string errorMessage(const T &v) { return mLimit.errorMessage(v); }

    LIMIT mLimit;
    GETTER mGetter;
    SETTER mSetter;
};

template <typename T, typename CLASS_T, typename SETTER_PARAM>
std::unique_ptr<ClassProperty> makeClassPropertyInstanceGS (T (CLASS_T::* getter)() const, void (CLASS_T::* setter)(SETTER_PARAM), const std::string &name) {
    return std::unique_ptr<ClassProperty>(new ClassPropertyInstanceGetterSetter<T, CLASS_T, NoLimit<T>, T (CLASS_T::*)() const, void (CLASS_T::*)(SETTER_PARAM)>(name, getter, setter, NoLimit<T>()));
}

template <typename T, typename CLASS_T, typename LIMIT, typename SETTER_PARAM>
std::unique_ptr<ClassProperty> makeClassPropertyInstanceGS (T (CLASS_T::* getter)() const, void (CLASS_T::* setter)(SETTER_PARAM), const std::string &name, LIMIT &&limit) {
    return std::unique_ptr<ClassProperty>(new ClassPropertyInstanceGetterSetter<T, CLASS_T, LIMIT, T (CLASS_T::*)() const, void (CLASS_T::*)(SETTER_PARAM)>(name, getter, setter, std::move(limit)));
}



template <class C>
struct ClassPropertyContainer {

};

#define PROPERTY_ACCESS \
    template<typename _C_> \
    friend struct ClassPropertyContainer;

#define BEGIN_CLASS_PROPERTY_LIST(_C_) \
    template <> struct ClassPropertyContainer<_C_> { \
        typedef _C_ ClassType; \
        static std::vector<std::unique_ptr<ClassProperty>> sPropertyList; \
        ClassPropertyContainer() { {



#define PROPERTY(_VAR_PTR_, _NAME_) \
    } {   \
        auto property = makeClassPropertyInstance((_VAR_PTR_),( _NAME_),( _LIMIT_)); \
        auto propertyPtr = property.get(); \
        sPropertyList.push_back(std::move(property));


#define PROPERTY_WITH_LIMIT(_VAR_PTR_, _NAME_, _LIMIT_) \
    } {   \
        auto property = makeClassPropertyInstance((_VAR_PTR_), (_NAME_), (_LIMIT_)); \
        auto propertyPtr = property.get(); \
        sPropertyList.push_back(std::move(property));

#define PROPERTY_GS(_GETTER_, _SETTER_, _NAME_) \
    } {   \
        auto property = makeClassPropertyInstanceGS((_GETTER_), (_SETTER_), (_NAME_)); \
        auto propertyPtr = property.get(); \
        sPropertyList.push_back(std::move(property));


#define PROPERTY_GS_WITH_LIMIT(_GETTER_, _SETTER_, _NAME_, _LIMIT_) \
    } {   \
        auto property = makeClassPropertyInstanceGS((_GETTER_), (_SETTER_), (_NAME_), (_LIMIT_)); \
        auto propertyPtr = property.get(); \
        sPropertyList.push_back(std::move(property));

#define LAST_PROPERTY_DEFAULT_VALUE(_VALUE_) \
        propertyPtr->mDefaultValue = (_VALUE_);


#define END_CLASS_PROPERTY_LIST() \
        } } \
    };

#define INSTANTIATE_CLASS_PROPERTIES(_C_) \
    std::vector<std::unique_ptr<ClassProperty>> ClassPropertyContainer<_C_>::sPropertyList; static ClassPropertyContainer<_C_> CLASS_PROPERTY_INITIALIZER ## __COUNTER__;

template <typename T>
std::vector<std::string> classPropertyNames() {
    std::vector<std::string> ret;
    for (const auto &p : ClassPropertyContainer<T>::sPropertyList) {
        ret.push_back(p->mName);
    }
    return ret;
}

