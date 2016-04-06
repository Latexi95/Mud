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
#include "traits/traitproperty.h"


class BaseItem {
public:
    BaseItem();
    BaseItem(const std::string &id);
    ~BaseItem();
    const Name &name() const;
    void setName(const Name &n);

    double weight() const { return mWeight; }
    void setWeight(double t) { mWeight = t; }
    const Box<int> &size() const { return mSize; }
    void setSize(const Box<int> &b) { mSize = b; }

    Json::Value serialize() const;
    void deserialize(const Json::Value &val);

    bool hasTrait(ItemTraitType traitType) const;
    template <typename TRAIT>
    bool hasTrait() const { return hasTrait(TRAIT::staticTraitType);}
    ItemTrait *trait(ItemTraitType type);
    template <typename TRAIT>
    TRAIT *trait() { return static_cast<TRAIT*>(trait(TRAIT::staticTraitType));}

    void addTrait(std::unique_ptr<ItemTrait> &&trait);
    std::unique_ptr<ItemTrait> removeTrait(ItemTraitType type);

    const std::unordered_map<unsigned, std::unique_ptr<ItemTrait> > &traits() const;

    void clone(BaseItem &copy) const;
    std::unique_ptr<BaseItem> clone() const;

    const std::string &id() const { return mId; }

    const std::string &description() const;
protected:
    std::string mId;
    std::string mDescription;
    Name mName;
    int mWeight; // grams
    Box<int> mSize; // mm
    std::unordered_map<unsigned, std::unique_ptr<ItemTrait> > mTraits;
};

class Item {
public:
    template<typename T>
    friend class Json::Serializer;
    Item();
    Item(const std::shared_ptr<BaseItem> &base);
    ~Item();

    const Name &name() const { return mBase->name(); }
    double weight() const { return mBase->weight(); }
    const Box<int> &size() const { return mBase->size(); }
    const std::string &description() const { return mBase->description(); }

    const ItemTrait *trait(ItemTraitType type) const { return mBase->trait(type); }
    template <typename TRAIT>
    const TRAIT *trait() const { return static_cast<TRAIT*>(mBase->trait(TRAIT::staticTraitType));}

    bool hasTrait(ItemTraitType type) const { return mBase->hasTrait(type); }
    template <typename TRAIT>
    bool hasTrait() const { return mBase->hasTrait(TRAIT::staticTraitType);}

    const std::shared_ptr<BaseItem> &base() const { return mBase; }

    template <typename FUNC>
    void foreachTrait(FUNC &&f) const;
private:
    std::shared_ptr<BaseItem> mBase;
    TraitPropertyValueMap mTraitPropertyValues;
};

template <typename FUNC>
void Item::foreachTrait(FUNC &&f) const {
    for (auto &traitPair : mBase->traits()) {
        f(traitPair.second.get());
    }
}


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

struct ItemSearchTextGetter
{
    std::string operator()(const Item *item) {
        return item->name().searchFormatted();
    }
};

#endif // ITEM_H
