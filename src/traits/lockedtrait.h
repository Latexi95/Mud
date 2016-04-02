#ifndef LOCKEDTRAIT_H
#define LOCKEDTRAIT_H
#include "itemtrait.h"

enum class LockState {
    Locked,
    Unlocked
};

class LockedTrait : public ItemTrait  {
public:
    template <typename TRAIT>
    friend class ItemTraitProperties;
    static const ItemTraitType staticTraitType = ItemTraitType::Locked;

    LockedTrait();
    virtual ~LockedTrait();
    virtual ItemTraitType type() const { return ItemTraitType::Locked; }
    virtual std::unique_ptr<ItemTrait> clone() const;
    virtual Json::Value serialize() const;
    virtual void deserialize(const Json::Value &val);
    virtual const char *traitName() const { return "locked"; }
    virtual void accept(ItemTraitVisitor *visitor);
    virtual void destructProperties(TraitPropertyValueMap &v) { }
    virtual Json::Value serializeProperties(const TraitPropertyValueMap &v);
    virtual void deserializeProperties(const Json::Value &obj, TraitPropertyValueMap &v);
private:
    TRAIT_PROPERTY(LockState, LockState);
};

TRAIT_PROPERTIES_ACCESSOR_BEGIN(LockedTrait)
    TRAIT_PROPERTIES_ACCESSOR_BASE(LockedTrait)
    TRAIT_PROPERTIES_ACCESSOR_COPY(LockState, LockState)
TRAIT_PROPERTIES_ACCESSOR_END()

namespace Json {
template<>
struct Serializer<LockState> {
    static Json::Value serialize(LockState s);
    static void deserialize(const Value &jsonVal, LockState &v);
};
}

#endif // LOCKEDTRAIT_H
