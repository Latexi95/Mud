#include "room.h"
#include "level.h"
#include "character.h"

Room::Room() :
    mLevel(nullptr)
{
}

Room::Room(const std::string &id):
    mId(id),
    mLevel(nullptr)
{
}

Room::Room(const std::string &id, const std::string &name, Level *level) :
    mName(name),
    mId(id),
    mLevel(level)
{
}

Room::~Room() {

}

std::vector<std::pair<Item *, Direction> > Room::items() const {
    std::vector<std::pair<Item*, Direction> > ret;
    for (unsigned side = 0; side < (unsigned)Direction::Count; ++side) {
        Direction dir = (Direction)side;
        const Wall &w = mWalls[side];
        for (auto &itemPtr : w.items()) {
            ret.emplace_back(itemPtr.get(), dir);
        }
    }
    return ret;
}

const std::vector<std::shared_ptr<Character> > &Room::characters() const {
    return mCharacters;
}

void Room::addCharacter(const std::shared_ptr<Character> &c)
{
    c->setRoom(this);
    mCharacters.push_back(c);
}

void Room::removeCharacter(const std::shared_ptr<Character> &c)
{
    for (auto it = mCharacters.begin(); it != mCharacters.end(); ++it) {
        if (*it == c) {
            mCharacters.erase(it);
            return;
        }
    }
}

const Wall &Room::wall(Direction side) const
{
    assert((int)side >= 0 && (int)side < (int)Direction::Count);
    return mWalls[(int)side];
}

Wall &Room::wall(Direction side)
{
    return mWalls[(int)side];
}

const std::array<Wall, (int)Direction::Count> &Room::walls() const
{
    return mWalls;
}

std::array<Wall, (int)Direction::Count> &Room::walls()
{
    return mWalls;
}

const std::vector<std::unique_ptr<RoomTrait>> &Room::traits() const {
    return mTraits;
}

void Room::addTrait(std::unique_ptr<RoomTrait> &&r)
{
    mTraits.emplace_back(std::move(r));
}

const TextSelectorMap<std::string> &Room::lookMap() const
{
    return mLookMap;
}

std::string Room::id() const
{
    return mId;
}

void Room::setId(const std::string &stringID)
{
    mId = stringID;
}

void Room::sendEventToCharacters(Event *e)
{
    for (auto &character : mCharacters) {
        character->handleEvent(e);
    }
}

RoomExit *Room::exit(Direction side)
{
    return wall(side).exit();
}

const std::string &Room::name() const
{
    return mName;
}

void Room::setName(const std::string &name)
{
    mName = name;
}

const std::string &Room::description() const
{
    return mDescription;
}

void Room::setDescription(const std::string &description)
{
    mDescription = description;
}

using namespace Json;

Value Serializer<Room>::serialize(const Room &r)
{
    Value ret(objectValue);
    ret["name"] = r.mName;
    ret["description"] = r.mDescription;

    Value wallsObj(objectValue);
    char c[2] = {0, 0};
    for (unsigned side = 0; side < (unsigned)Direction::Count; ++side) {
        Direction dir = (Direction)side;
        const Wall &w = r.mWalls[side];
        c[0] = DirectionToChar(dir);
        wallsObj[c] = Json::serialize(w);
    }
    ret["walls"] = wallsObj;

    if (!r.mTraits.empty()) {
        Value traitsArray(arrayValue);
        for (const std::unique_ptr<RoomTrait> &t : r.mTraits) {
            traitsArray.append(Json::serialize(t));
        }
        ret["traits"] = traitsArray;
    }

    if (!r.mLookMap.empty()) {
        Value lookObject(objectValue);
        for (const auto &lookPair : r.mLookMap) {
            lookObject[lookPair.first] = lookPair.second;
        }
        ret["looks"] = lookObject;
    }

    if (!r.mCharacters.empty()) {
        Json::Value charactersArray(Json::arrayValue);
        for (const std::shared_ptr<Character> &c : r.mCharacters) {
            if (c->saveWithLevel()) {
                charactersArray.append(Json::serialize(*c));
            }
        }
        ret["characters"] = charactersArray;
    }

    return ret;
}

void Serializer<Room>::deserialize(const Value &v, Room &r)
{
    if (!v.isObject()) throw SerializationException("Serializer<Room>::deserialize: expecting an json object");

    Json::deserialize(v["name"], r.mName);
    Json::deserialize(v["description"], r.mDescription);

    Value wallsObj = v["walls"];
    if (!wallsObj.isObject())
        throw SerializationException("Serializer<Room>::deserialize: 'walls' field doesn't exist or is invalid");

    char c[2] = {0, 0};
    for (unsigned i = 0; i < (unsigned)Direction::Count; ++i) {

        c[0] = DirectionToChar((Direction)i);
        Value wallObj = wallsObj[c];
        if (!wallObj.isObject()) {
            wallObj = wallsObj[DirectionToString((Direction)i)];
        }
        Json::deserialize(wallObj, r.mWalls[i]);
    }

    Value traitsArray = v["traits"];
    if (traitsArray.isArray()) {
        for (const Value &v : traitsArray) {
            r.mTraits.emplace_back();
            Json::deserialize(v, r.mTraits.back());
        }
    }

    Value looks = v["looks"];
    if (looks.isObject()) {
        for (auto lookIt = looks.begin(); lookIt != looks.end(); ++lookIt) {
            std::string text;
            Json::deserialize(*lookIt, text);
            r.mLookMap.insert(lookIt.name(), text);
        }
    }

    Json::deserialize(v["characters"], r.mCharacters);
    for (auto &character : r.mCharacters) {
        character->setRoom(&r);
    }

}


