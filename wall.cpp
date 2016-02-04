#include "wall.h"
#include <cassert>
#include "item.h"

Wall::Wall()
{

}


Wall::~Wall()
{
}

const std::vector<std::unique_ptr<Item> > &Wall::items() const
{
    return mItems;
}

void Wall::addItem(std::unique_ptr<Item> &&i)
{
    mItems.emplace_back(std::move(i));
}


const std::string &Wall::looks() const
{
    return mLooks;
}

RoomExit *Wall::exit() const
{
    return mExit.get();
}

void Wall::setExit(std::unique_ptr<RoomExit> &&e)
{
    mExit = std::move(e);
}

bool Wall::hasExit() const
{
    return mExit != nullptr;
}


void Wall::setLooks(const std::string &looks)
{
    mLooks = looks;
}

using namespace Json;
Value Serializer<Wall>::serialize(const Wall &w)
{
    Value ret(objectValue);
    ret["looks"] = Json::serialize(w.mLooks);

    if (!w.mItems.empty()) {
        ret["items"] = Json::serialize(w.mItems);
    }

    if (w.hasExit()) {
        ret["exit"] = Json::serialize(*w.exit());
    }

    return ret;
}

void Serializer<Wall>::deserialize(const Value &v, Wall &w)
{
    Json::deserialize(v["looks"], w.mLooks);
    Json::deserialize(v["items"], w.mItems);

    Value exitValue = v["exit"];
    if (exitValue.isObject()) {
        w.mExit = std::unique_ptr<RoomExit>(new RoomExit());
        Json::deserialize(exitValue, *w.mExit);
    }
}
