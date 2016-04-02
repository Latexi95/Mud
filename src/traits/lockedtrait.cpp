#include "lockedtrait.h"
#include "util/textutils.h"

LockedTrait::LockedTrait() :
    ItemTrait(),
    mPropertyLockState("lockState")
{
}

LockedTrait::~LockedTrait()
{

}

std::unique_ptr<ItemTrait> LockedTrait::clone() const {
    std::unique_ptr<ItemTrait> trait = std::unique_ptr<ItemTrait>(new LockedTrait());

    return trait;
}

Json::Value LockedTrait::serialize() const {
    return Json::Value();
}

void LockedTrait::deserialize(const Json::Value &val) {

}

void LockedTrait::accept(ItemTraitVisitor *visitor)
{
    visitor->visit(this);
}

Json::Value LockedTrait::serializeProperties(const TraitPropertyValueMap &v)
{
    return mPropertyLockState.serialize(v);
}

void LockedTrait::deserializeProperties(const Json::Value &obj, TraitPropertyValueMap &v)
{
    mPropertyLockState.deserialize(v, obj);
}

Json::Value Json::Serializer<LockState>::serialize(LockState s)
{
    switch (s) {
    case LockState::Locked:
        return "locked";
    case LockState::Unlocked:
        return "unlocked";
    }
}

void Json::Serializer<LockState>::deserialize(const Json::Value &jsonVal, LockState &v)
{
    if (jsonVal.isString()) {
        std::string txt = text::lowered(jsonVal.asString());
        if (txt == "locked") {
            v = LockState::Locked;
        } else if (txt == "unlocked") {
            v = LockState::Unlocked;
        }
        else {
            throw SerializationException("Serializer<LockState>::deserialize: Expecting 'locked'/'unlocked'");
        }
    }
    else if (jsonVal.isBool()) {
        v = jsonVal.asBool() ? LockState::Locked : LockState::Unlocked;
    }
    else {
        throw SerializationException("Serializer<LockState>::deserialize: Expecting a string or boolean");
    }
}
