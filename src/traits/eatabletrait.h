#ifndef EATABLETRAIT_H
#define EATABLETRAIT_H
#include "itemtrait.h"

class EatableTrait : public ItemTrait
{
public:
    static const ItemTraitType staticTraitType = ItemTraitType::Eatable;

    EatableTrait();
    virtual ~EatableTrait();
    virtual ItemTraitType type() const { return staticTraitType; }
    virtual std::unique_ptr<ItemTrait> clone() const;
    virtual Json::Value serialize() const;
    virtual void deserialize(const Json::Value &val);
    virtual const char *traitName() const { return "eatable"; }
    virtual void accept(ItemTraitVisitor *visitor);

protected:

};

#endif // EATABLETRAIT_H
