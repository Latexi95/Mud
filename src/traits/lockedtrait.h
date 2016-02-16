#ifndef LOCKEDTRAIT_H
#define LOCKEDTRAIT_H
#include "itemtrait.h"
class LockedTrait : public ItemTrait  {
public:
    static const ItemTraitType staticTraitType = ItemTraitType::Locked;

    LockedTrait();
    virtual ~LockedTrait();
    virtual ItemTraitType type() const { return ItemTraitType::Locked; }
    virtual std::unique_ptr<ItemTrait> clone() const;
    virtual Json::Value serialize() const;
    virtual void deserialize(const Json::Value &val);
    virtual const char *traitName() const { return "locked"; }
    virtual void accept(ItemTraitVisitor *visitor);
};

#endif // LOCKEDTRAIT_H
