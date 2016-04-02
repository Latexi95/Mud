#ifndef ITEMTRAIT_H
#define ITEMTRAIT_H
#include "util/jsonserializable.h"
#include <memory>
#include <boost/algorithm/string.hpp>
#include "util/enums.h"
#include "traitproperty.h"

class ContainerTrait;
class LandmarkTrait;
class LockedTrait;
class WeaponTrait;
class EatableTrait;
class ItemTraitVisitor {
public:
    virtual void visit(ContainerTrait *t);
    virtual void visit(LandmarkTrait *t);
    virtual void visit(LockedTrait *t);
    virtual void visit(WeaponTrait *t);
    virtual void visit(EatableTrait *t);
};


template <typename TRAIT>
struct ItemTraitProperties {
    ItemTraitProperties(TRAIT *trait, TraitPropertyValueMap &m) : mTrait(trait), mValueMap(m) {}
protected:
    TRAIT *mTrait;
    TraitPropertyValueMap &mValueMap;
};


class ItemTrait {
public:
    ItemTrait();
    virtual ~ItemTrait();
    virtual std::unique_ptr<ItemTrait> clone() const = 0;
    virtual ItemTraitType type() const = 0;
    virtual const char *traitName() const = 0;

    static std::unique_ptr<ItemTrait> createItemTraitByType(ItemTraitType type);
    static std::unique_ptr<ItemTrait> createItemTraitByName(const std::string &name);

    virtual Json::Value serialize() const = 0;
    virtual void deserialize(const Json::Value &val) = 0;

    virtual void accept(ItemTraitVisitor *visitor) = 0;

    virtual void destructProperties(TraitPropertyValueMap &v) { }
    virtual Json::Value serializeProperties(const TraitPropertyValueMap &v) {}
    virtual void deserializeProperties(const Json::Value &obj, TraitPropertyValueMap &v) {}
protected:
    void serializeBase(Json::Value &val) const;
};

namespace Json {

template<>
struct Serializer<std::unique_ptr<ItemTrait> > {
    static Json::Value serialize(const std::unique_ptr<ItemTrait> &trait) {
        Json::Value val = trait->serialize();
        val["traitname"] = trait->traitName();
        return val;
    }

    static void deserialize(const Value &val, std::unique_ptr<ItemTrait> &trait) {
        Json::Value traitName = val.get("traitname", Json::Value());
        if (!traitName.isString()) { throw SerializationException("Expecting \"traitname\" string field in deserialize<std::unique_ptr<ItemTrait>"); }
        std::string name = traitName.asString();
        boost::algorithm::to_lower(name);
        trait = ItemTrait::createItemTraitByName(name);
        if (!trait) throw SerializationException("Invalid traitname \"" + name + "\"");
        trait->deserialize(val);
    }
};

}

#define TRAIT_PROPERTIES_ACCESSOR_BEGIN(_TRAIT_) \
template<> \
struct ItemTraitProperties<_TRAIT_> {

#define TRAIT_PROPERTIES_ACCESSOR_END() };

#define TRAIT_PROPERTIES_ACCESSOR_BASE(_TRAIT_) \
    public: ItemTraitProperties<_TRAIT_>(_TRAIT_ *trait, TraitPropertyValueMap &p) : mTrait(trait), mValueMap(p) {} \
    private: _TRAIT_ *mTrait; TraitPropertyValueMap &mValueMap;

#define TRAIT_PROPERTIES_ACCESSOR_COPY(_PROPERTY_NAME_, _TYPE_) \
    public: void set ## _PROPERTY_NAME_ (const _TYPE_ &v) { (mTrait->mProperty ## _PROPERTY_NAME_).store(mValueMap, v);} \
    _TYPE_ &get ## _PROPERTY_NAME_ () { return (mTrait->mProperty ## _PROPERTY_NAME_).load(mValueMap); }

#define TRAIT_PROPERTIES_ACCESSOR_MOVE(_PROPERTY_NAME_, _TYPE_) \
    public: void set ## _PROPERTY_NAME_ (_TYPE_ &&v) { (mTrait->mProperty ## _PROPERTY_NAME_).store(mValueMap, std::move(v));} \
    _TYPE_ &get ## _PROPERTY_NAME_ () { return (mTrait->mProperty ## _PROPERTY_NAME_).load(mValueMap); }

#define TRAIT_PROPERTY(_PROPERTY_NAME_, _TYPE_) \
    TraitProperty<_TYPE_> mProperty##_PROPERTY_NAME_



#endif // ITEMTRAIT_H
