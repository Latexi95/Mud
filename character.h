#ifndef CHARACTER_H
#define CHARACTER_H
#include "item.h"
#include "jsonserializable.h"
#include <map>
#include "position.h"
#include "textgen/color.h"

class Level;
class Event;
class EventHandler;

class Character : public JsonSerializable {
public:
    enum Gender {
        Male,
        Female
    };

    Character();
    Character(const std::string &name, Gender gender = Male);
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

    std::shared_ptr<Level> level() const { return mLevel; }
    void setLevel(const std::shared_ptr<Level> &level) { mLevel = level; }
    const Position &pos() const { return mPos; }
    void setPos(const Position &pos) { mPos = pos; }

    text::Color *hairColor() const { return mHairColor; }
    void setHairColor(text::Color *c) { mHairColor = c; }

    virtual Json::Value serialize() const;
    virtual void deserialize(const Json::Value &val);

    void handleEvent(Event *e);

    void addEventHandler(std::unique_ptr<EventHandler> &&eventHandler);
    void removeEventHandlers();
protected:
    std::string mName;
    Gender mGender;
    int mAge;

    double mHeight;
    double mWidth;

    int mStrength;
    int mDexterity;
    int mConstitution;
    int mIntelligence;
    int mWisdom;
    int mCharisma;

    text::Color *mHairColor;
    Position mPos;

    std::shared_ptr<Level> mLevel;
    std::vector<std::unique_ptr<EventHandler> > mEventHandlers;



    std::map<std::string, int> mSkillLevels;
    std::map<std::string, std::vector<std::unique_ptr<Item> > > mEquipment;
};

#endif // CHARACTER_H
