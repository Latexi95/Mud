#include "eatabletrait.h"

EatableTrait::EatableTrait() :
    mEnergy(0)
{

}

EatableTrait::~EatableTrait()
{

}

std::unique_ptr<ItemTrait> EatableTrait::clone() const
{

}

Json::Value EatableTrait::serialize() const
{
    Json::Value ret(Json::objectValue);
    serializeBase(ret);
    ret["energy"] = mEnergy;
    return ret;
}

void EatableTrait::deserialize(const Json::Value &val)
{
    Json::deserialize(val["energy"], mEnergy);
}

void EatableTrait::accept(ItemTraitVisitor *visitor)
{

}

int EatableTrait::energy() const
{
    return mEnergy;
}

void EatableTrait::setEnergy(int energy)
{
    mEnergy = energy;
}
