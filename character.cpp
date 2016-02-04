#include "character.h"
#include <algorithm>
#include "resourceservice.h"
#include "level.h"
#include "levelservice.h"
#include "events/event.h"
#include "charactereventhandler.h"
#include "levelservice.h"

Character::Character() :
    mGender(Male),
    mAge(25),
    mSaveWithLevel(true),
    mHeight(1.8),
    mWidth(0.5),
    mStrength(10),
    mDexterity(10),
    mConstitution(10),
    mIntelligence(10),
    mWisdom(10),
    mCharisma(10),
    mHairColor(0)
{

}
Character::Character(const std::string &id) :
    mId(id),
    mName(id),
    mGender(Male),
    mAge(25),
    mSaveWithLevel(true),
    mHeight(1.8),
    mWidth(0.5),
    mStrength(10),
    mDexterity(10),
    mConstitution(10),
    mIntelligence(10),
    mWisdom(10),
    mCharisma(10)
{
}

Character::~Character() {

}

void Character::setSkillLevel(std::string skillName, int level) {
    std::transform(skillName.begin(), skillName.end(), skillName.begin(), ::tolower);
    mSkillLevels[skillName] = level;
}

int Character::skillLevel(std::string skillName) const {
    std::transform(skillName.begin(), skillName.end(), skillName.begin(), ::tolower);
    auto i = mSkillLevels.find(skillName);
    if (i != mSkillLevels.end()) {
        return i->second;
    }
    return 0;
}

void Character::handleEvent(Event *e)
{
    std::shared_ptr<Character> self = shared_from_this();
    for (const std::unique_ptr<CharacterEventHandler> &eventHandler : mEventHandlers) {
        eventHandler->handleEvent(self, e);
    }
}

void Character::addEventHandler(std::unique_ptr<CharacterEventHandler> &&eventHandler) {
    mEventHandlers.emplace_back(std::move(eventHandler));
}

void Character::removeEventHandlers()
{
    mEventHandlers.clear();
}

Room *Character::room() const
{
    return mRoom;
}

void Character::setRoom(Room *room)
{
    mRoomId = room->id();
    mLevelId = room->level()->id();
    mRoom = room;
}

const std::shared_ptr<LevelEventQueue> &Character::eventQueue() const
{
    return mRoom->level()->eventQueue();
}

const std::string &Character::id() const
{
    return mId;
}

void Character::setId(const std::string &id)
{
    mId = id;
}

void Character::setLevelAndRoomIds(const std::string &levelId, const std::string &roomId)
{
    mLevelId = levelId;
    mRoomId = roomId;
}

void Character::placeToLocation()
{
    auto level = LS->level(mLevelId);
    if (!level) {
        throw SerializationException("Character::placeToLocation: Unknown level \"" + mLevelId + "\"");
    }
    if (!level->hasRoomById(mRoomId)) {
        throw SerializationException("Character::placeToLocation: Unknown room \"" + mRoomId + "\"");
    }
    auto room = level->roomById(mRoomId);
    room->addCharacter(shared_from_this());
}

std::string Character::roomId() const
{
    return mRoomId;
}

std::string Character::levelId() const
{
    return mLevelId;
}

bool Character::saveWithLevel() const
{
    return mSaveWithLevel;
}

void Character::setSaveWithLevel(bool saveWithLevel)
{
    mSaveWithLevel = saveWithLevel;
}

Level *Character::level() const
{
    if (!mRoom) {
        return LS->level(mLevelId);
    }
    return mRoom->level();
}

Json::Value Json::Serializer<Character>::serialize(const Character &c)
{
    Json::Value obj(Json::objectValue);
    obj["str"] = c.mStrength;
    obj["dex"] = c.mDexterity;
    obj["con"] = c.mConstitution;
    obj["int"] = c.mIntelligence;
    obj["wis"] = c.mWisdom;
    obj["cha"] = c.mCharisma;
    obj["height"] = c.mHeight;
    obj["width"] = c.mWidth;
    obj["name"] = c.mName;
    obj["id"] = c.mId;

    obj["level"] = c.mLevelId;
    obj["room"] = c.mRoomId;

    obj["skills"] = Json::serialize(c.mSkillLevels);
    obj["equipment"] = Json::serialize(c.mEquipment);
    return obj;
}

void Json::Serializer<Character>::deserialize(const Json::Value &v, Character &c)
{
    Json::deserialize(v["str"], c.mStrength);
    Json::deserialize(v["dex"], c.mDexterity);
    Json::deserialize(v["con"], c.mConstitution);
    Json::deserialize(v["int"], c.mIntelligence);
    Json::deserialize(v["wis"], c.mWisdom);
    Json::deserialize(v["cha"], c.mCharisma);
    Json::deserialize(v["height"], c.mHeight);
    Json::deserialize(v["width"], c.mWidth);
    Json::deserialize(v["name"], c.mName);
    Json::deserialize(v["id"], c.mId);

    Json::deserialize(v["level"], c.mLevelId);
    Json::deserialize(v["room"], c.mRoomId);

    Json::deserialize(v["skills"], c.mSkillLevels);
    Json::deserialize(v["equipment"], c.mEquipment);

}
