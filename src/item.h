#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <boost/variant.hpp>

#include "util/name.h"
#include "util/box.h"
#include "util/defines.h"
#include "util/enums.h"
#include "traits/itemtrait.h"
#include "traitproperty.h"


class BaseItem {
public:
    BaseItem();
    BaseItem(const std::string &id);
    ~BaseItem();
    const Name &name() const;
    void setName(const Name &n);

    double weight() const { return mWeight; }
    void setWeight(double t) { mWeight = t; }
    const Box<float> &size() const { return mSize; }
    void setSize(const Box<float> &b) { mSize = b; }

    Json::Value serialize() const;
    void deserialize(const Json::Value &val);

    bool hasTrait(ItemTraitType traitType);

    ItemTrait *trait(ItemTraitType type);
    void addTrait(std::unique_ptr<ItemTrait> &&trait);
    std::unique_ptr<ItemTrait> removeTrait(ItemTraitType type);

    const std::unordered_map<unsigned, std::unique_ptr<ItemTrait> > &traits() const;

    void clone(BaseItem &copy) const;
    std::unique_ptr<BaseItem> clone() const;

    std::string id() const { return mId; }
protected:
    std::string mId;
    Name mName;
    float mWeight;
    Box<float> mSize;
    std::unordered_map<unsigned, std::unique_ptr<ItemTrait> > mTraits;
};

class Item {
public:
    Item(const std::shared_ptr<BaseItem> &base);
    ~Item();

    const Name &name() const { return mBase->name(); }
    double weight() const { return mBase->weight(); }
    const Box<float> &size() const { return mBase->size(); }

    const ItemTrait *trait(ItemTraitType type) const { return mBase->trait(type); }
    template <typename TRAIT>
    const TRAIT *trait() const { return static_cast<TRAIT*>(mBase->trait(TRAIT::staticTraitType));}

    bool hasTrait(ItemTraitType type) const { return mBase->hasTrait(type); }
    template <typename TRAIT>
    bool hasTrait() const { return mBase->hasTrait(TRAIT::staticTraitType);}
private:
    std::shared_ptr<BaseItem> mBase;
    TraitPropertyValueMap mTraitPropertyValues;
};

namespace Json {
template<>
struct Serializer<BaseItem> {
    static Value serialize(const BaseItem &i);
    static void deserialize(const Value &v, BaseItem &i);
};

template<>
struct Serializer<Item> {
    static Value serialize(const Item &i);
    static void deserialize(const Value &v, Item &i);
};
}

#endif // ITEM_H
