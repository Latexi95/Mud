#include "eatabletrait.h"

EatableTrait::EatableTrait()
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

}

void EatableTrait::deserialize(const Json::Value &val)
{

}

void EatableTrait::accept(ItemTraitVisitor *visitor)
{

}
