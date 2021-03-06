#ifndef LOCKEDTRAIT_H
#define LOCKEDTRAIT_H
#include "itemtrait.h"
class LockedTrait : public ItemTrait  {
public:
    LockedTrait();

    ItemTraitType type() const { return ItemTraitType::Locked; }
    std::unique_ptr<ItemTrait> clone() const;
    Json::Value serialize() const;
    void deserialize(const Json::Value &val);
    const char *traitName() const { return "locked"; }
};

#endif // LOCKEDTRAIT_H
