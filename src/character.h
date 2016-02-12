#ifndef CHARACTER_H
#define CHARACTER_H
#include "item.h"
#include "jsonserializable.h"
#include <map>
#include "position.h"
#include "textgen/color.h"

class Level;
class Room;
class Event;
class LevelEventQueue;
class CharacterEventHandler;

class Character : public std::enable_shared_from_this<Character> {
    template <typename T>
    friend struct Json::Serializer;
public:
    enum Gender {
        Male,
        Female
    };

    Character();
    Character(const std::string &id);
    ~Character();

    const std::string &name() const { return mName; }
    void setName(const std::string &name) { mName = name; }

    double height() const { return mHeight; }
    void setHeight(double h) { mHeight = h; }
    double width() const { return mWidth; }
    void setWidth(double w) { mWidth = w; }

    Gender gender() const { return mGender; }
    void setGender(Gender gender) { mGender = gender; }

    int age() const { return mAge; }
    void setAge(int age) { mAge = age; }

    int strength() const { return mStrength; }
    void setStrength(int str) { mStrength = str; }
    int dexterity() const { return mDexterity; }
    void setDexterity(int dex) { mDexterity = dex; }
    int constitution() const { return mConstitution; }
    void setConstitution(int con) { mConstitution = con; }
    int intelligence() const { return mIntelligence; }
    void setIntelligence(int i) { mIntelligence = i; }
    int wisdom() const { return mWisdom; }
    void setWisdom(int wis) { mWisdom = wis; }
    int charisma() const { return mCharisma; }
    void setCharisma(int cha) { mCharisma = cha; }

    int STR() const { return (mStrength - 10) / 2; }
    int DEX() const { return (mDexterity - 10) / 2; }
    int CON() const { return (mConstitution - 10) / 2; }
    int INT() const { return (mIntelligence - 10) / 2; }
    int WIS() const { return (mWisdom - 10) / 2; }
    int CHA() const { return (mCharisma - 10) / 2; }

    void setSkillLevel(std::string skillName, int level);
    int skillLevel(std::string skillName) const;

    text::Color *hairColor() const { return mHairColor; }
    void setHairColor(text::Color *c) { mHairColor = c; }

    void handleEvent(Event *e);

    void addEventHandler(std::unique_ptr<CharacterEventHandler> &&eventHandler);
    void removeEventHandlers();

    Room *room() const;
    void setRoom(Room *room);
    Level *level() const;
    LevelEventQueue *eventQueue() const;
    const std::string &id() const;
    void setId(const std::string &id);
    
    void setLevelAndRoomIds(const std::string &levelId, const std::string &roomId);

    void placeToLocation();
    std::string roomId() const;
    std::string levelId() const;

    bool saveWithLevel() const;
    void setSaveWithLevel(bool saveWithLevel);

protected:
    std::string mId;
    std::string mName;
    Gender mGender;
    int mAge;

    bool mSaveWithLevel;

    float mHeight;
    float mWidth;

    int mStrength;
    int mDexterity;
    int mConstitution;
    int mIntelligence;
    int mWisdom;
    int mCharisma;

    text::Color *mHairColor;
    std::string mRoomId;
    std::string mLevelId;
    Room *mRoom;

    std::vector<std::unique_ptr<CharacterEventHandler> > mEventHandlers;

    std::unordered_map<std::string, int> mSkillLevels;
    std::unordered_map<std::string, std::vector<std::unique_ptr<Item> > > mEquipment;
};

namespace Json {
template <>
struct Serializer<Character> {
    static Value serialize(const Character &c);
    static void deserialize(const Value &v, Character &c);
};
}


#endif // CHARACTER_H
