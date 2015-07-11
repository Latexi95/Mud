#include "level.h"
#include <unordered_set>
#include <boost/lexical_cast.hpp>
#include "item.h"

Level::Level(const std::string &id) :
    mId(id),
    mRoomData(0),
    mEventQueue(std::make_shared<LevelEventQueue>())
{
}

Level::~Level() {
    delete [] mRoomData;
}

void Level::init() {
    mRoomData = new std::list<RoomData>::iterator [mWidth * mHeight];
}

const std::string &Level::name() const {
    return mName;
}

void Level::setName(const std::string &value) {
    mName = value;
}

const std::string &Level::id() const {
    return mId;
}

Room Level::room(int localX, int localY) {
    return Room(localX, localY, this);
}

Json::Value Level::serialize() const {
    Json::Value root(Json::objectValue);
    root["name"] = mName;
    root["width"] = mWidth;
    root["height"] = mHeight;
    root["roomsize"] = mRoomSize;

    std::unordered_map<const RoomData*, int> uniqueRooms;
    std::unordered_map<const WallData*, int> uniqueWalls;
    int roomId = 1;
    int wallId = 1;
    for (const RoomData &roomData : mUniqueRoomData) {
        uniqueRooms[&roomData] = roomId++;
    }

    for (const WallData &wallData : mUniqueWallData) {
        uniqueWalls[&wallData] = wallId++;
    }

    Json::Value rooms(Json::objectValue);
    Json::Value walls(Json::objectValue);

    for (const std::pair<const WallData*, int> &wallIdPair : uniqueWalls) {
        const WallData *wallData = wallIdPair.first;
        Json::Value wallObj(Json::objectValue);
        wallObj["looks"] = wallData->mLooks;
        wallObj["solid"] = wallData->mSolid;
        Json::Value itemArray(Json::arrayValue);
        for (auto &itemPtr : wallData->mItems) {
            itemArray.append(itemPtr->serialize());
        }
        if (itemArray.size()) {
            wallObj["items"] = itemArray;
        }

        walls[boost::lexical_cast<std::string>(wallIdPair.second)] = wallObj;
    }

    for (const std::pair<const RoomData *, int> &roomIdPair : uniqueRooms) {
        const RoomData *roomData = roomIdPair.first;
        Json::Value roomObj(Json::objectValue);
        Json::Value looksObj(Json::objectValue);
        for (const std::pair<std::string, std::string> &look : roomData->mLooks) {
            looksObj[look.first] = look.second;
        }
        if (looksObj.size()) {
            roomObj["looks"] = looksObj;
        }

        roomObj["solid"] = roomData->mSolid;

        Json::Value wallsObj(Json::objectValue);
        for (int i = 0; i < Wall::SideCount; ++i) {
            std::string wallId = boost::lexical_cast<std::string>(uniqueWalls[&*roomData->mWalls[i]]);
            switch (i) {
            case Wall::North:
                wallsObj["n"] = wallId; break;
            case Wall::South:
                wallsObj["s"] = wallId; break;
            case Wall::East:
                wallsObj["e"] = wallId; break;
            case Wall::West:
                wallsObj["w"] = wallId; break;
            case Wall::Up:
                wallsObj["u"] = wallId; break;
            case Wall::Down:
                wallsObj["d"] = wallId; break;
            default:
                assert("Invalid wall side" && 0);
            }
        }
        roomObj["walls"] = wallsObj;

        rooms[boost::lexical_cast<std::string>(roomIdPair.second)] = roomObj;
    }
    root["rooms"] = rooms;
    root["walls"] = walls;

    Json::Value roomArray(Json::arrayValue);
    for (unsigned y = 0; y < mHeight; ++y) {
        Json::Value lineArray(Json::arrayValue);
        for (unsigned x = 0; x < mWidth; ++x) {
            lineArray.append(boost::lexical_cast<std::string>(uniqueRooms.at(roomData(x, y))));
        }
        roomArray.append(lineArray);
    }
    root["data"] = roomArray;
    return root;
}

void Level::deserialize(const Json::Value &root) {
    mWidth = root.get("width", 1).asInt();
    mHeight = root.get("height", 1).asInt();
    mName = root.get("name", "level").asString();
    mRoomSize = root.get("roomsize", 10).asDouble();
    init();

    Json::Value walls = root["walls"];
    std::unordered_map<std::string, std::list<WallData>::iterator> wallDataMap;
    for (Json::Value::iterator i = walls.begin(); i != walls.end(); ++i) {
        mUniqueWallData.emplace_back();
        std::list<WallData>::iterator data = --mUniqueWallData.end();
        data->mRefCount = 0;

        data->mLooks = i->get("looks", "").asString();
        data->mSolid = i->get("solid", true).asBool();

        Json::Value items = i->get("items", Json::Value());
        if (items.isArray()) {
            for (Json::Value::iterator itemIt = items.begin(); itemIt != items.end(); ++itemIt) {
                std::unique_ptr<Item> item = std::unique_ptr<Item>(new Item());
                item->deserialize(*itemIt);
                data->mItems.emplace_back(std::move(item));
            }
        }

        wallDataMap[i.memberName()] = data;
    }

    std::unordered_map<std::string, std::list<RoomData>::iterator> roomDataMap;

    Json::Value rooms = root["rooms"];
    for (Json::Value::iterator i = rooms.begin(); i != rooms.end(); ++i) {
        mUniqueRoomData.emplace_back();
        std::list<RoomData>::iterator data = --mUniqueRoomData.end();

        Json::Value looks = i->get("looks", Json::Value());
        if (looks.isObject()) {
            for (Json::Value::iterator looksIt = looks.begin(); looksIt != looks.end(); ++looksIt) {
                data->mLooks[looksIt.memberName()] = looksIt->asString();
            }
        }

        data->mSolid = i->get("solid", true).asBool();

        Json::Value walls = i->get("walls", Json::Value());
        Json::Value wallValues[Wall::SideCount];
        wallValues[Wall::North] = walls["n"];
        wallValues[Wall::South] = walls["s"];
        wallValues[Wall::West] = walls["w"];
        wallValues[Wall::East] = walls["e"];
        wallValues[Wall::Up] = walls["u"];
        wallValues[Wall::Down] = walls["d"];
        for (int i = 0; i < Wall::SideCount; ++i) {
            data->mWalls[i] = wallDataMap[wallValues[i].asCString()];
            ++data->mWalls[i]->mRefCount;
        }
        roomDataMap[i.memberName()] = data;
    }

    Json::Value data = root["data"];
    if (data.size() != mHeight)
        throw SerialiazationException("Level data array lenght doesn't match level height");
    int y = 0;
    for (Json::Value::iterator lineIt = data.begin(); lineIt != data.end(); ++lineIt) {
        int x = 0;
        if (lineIt->size() != mWidth)
            throw SerialiazationException("Level data array lenght doesn't match level width");
        for (Json::Value::iterator i = lineIt->begin(); i != lineIt->end(); ++i) {
            std::string roomId = i->asString();
            mRoomData[x + y * mWidth] = roomDataMap[roomId];
            ++mRoomData[x + y * mWidth]->mRefCount;
            ++x;
        }
        ++y;
    }
}

RoomData *Level::roomData(int x, int y) const {
    return &(*mRoomData[x + mWidth * y]);
}

void Level::addCharacter(const std::shared_ptr<Character> &c) {
    mCharacters.insert(std::pair<Position, std::shared_ptr<Character> >(c->pos(), c));
}

void Level::removeCharacter(const std::shared_ptr<Character> &c)
{
    auto range = mCharacters.equal_range(c->pos());
    for (auto it = range.first; it != range.second; ++it) {
        if (it->second == c) {
            mCharacters.erase(it);
            return;
        }
    }
}

void Level::moveCharacter(const std::shared_ptr<Character> &c, const Position &pos) {
    mCharacters.erase(c->pos());
    mCharacters.insert(std::pair<Position, std::shared_ptr<Character> >(pos, c));
    c->setPos(pos);
}

void Level::sendEventToCharacters(Event *e) {
    for (const std::pair<Position, std::shared_ptr<Character> >  &posCharPair : mCharacters) {
        posCharPair.second->handleEvent(e);
    }
}


