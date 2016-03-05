#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "util/name.h"
#include "util/box.h"
#include "util/defines.h"
#include "util/enums.h"
#include "traits/itemtrait.h"

class Item {
public:
    Item();
    Item(const std::string &id);
    ~Item();
    void initFromBase(const std::shared_ptr<Item> &b);
    const Name &name() const;
    void setName(const Name &n);

    double weight() const { return mWeight; }
    void setWeight(double t) { mWeight = t; }
    const Box<float> &size() const { return mSize; }
    void setSize(const Box<float> &b) { mSize = b; }

    Json::Value serialize() const;
    void deserialize(const Json::Value &val);

    bool hasTrait(ItemTraitType traitType);
    template <typename TraitT>
    bool hasTrait();

    ItemTrait *trait(ItemTraitType type);
    template <typename TraitT>
    TraitT *trait();
    void addTrait(std::unique_ptr<ItemTrait> &&trait);
    std::unique_ptr<ItemTrait> removeTrait(ItemTraitType type);

    const std::unordered_map<unsigned, std::unique_ptr<ItemTrait> > &traits() const;

    void clone(Item &copy) const;
    std::unique_ptr<Item> clone() const;

    std::string id() const { return mId; }
protected:
    std::shared_ptr<Item> mBase;
    std::string mId;
    Name mName;
    float mWeight;
    Box<float> mSize;
    std::unordered_map<unsigned, std::unique_ptr<ItemTrait> > mTraits;
};

template <typename TraitT>
bool Item::hasTrait() {
    return mTraits.find((unsigned)TraitT::staticTraitType) != mTraits.end();
}

template <typename TraitT>
TraitT *Item::trait() {
    auto it = mTraits.find((unsigned)TraitT::staticTraitType);
    if (it == mTraits.end()) return nullptr;
    return static_cast<TraitT*>(it->second.get());
}

namespace Json {
template<>
struct Serializer<Item> {
    static Value serialize(const Item &i);
    static void deserialize(const Value &v, Item &i);
};
}

#endif // ITEM_H
