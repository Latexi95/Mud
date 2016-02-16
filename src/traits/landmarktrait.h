#ifndef LANDMARKTRAIT_H
#define LANDMARKTRAIT_H
#include "itemtrait.h"

class LandmarkTrait : public ItemTrait {
public:
    static const ItemTraitType staticTraitType = ItemTraitType::Landmark;

    LandmarkTrait();
    virtual ~LandmarkTrait();
    virtual ItemTraitType type() const { return ItemTraitType::Landmark; }
    virtual std::unique_ptr<ItemTrait> clone() const;
    virtual Json::Value serialize() const;
    virtual void deserialize(const Json::Value &val);
    virtual const char *traitName() const { return "landmark"; }
    virtual void accept(ItemTraitVisitor *visitor);
};

#endif // LANDMARKTRAIT_H
