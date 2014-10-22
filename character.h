#ifndef CHARACTER_H
#define CHARACTER_H
#include "item.h"
#include <map>
class Character : public Item {
	public:
		Character(const std::string &name);
		~Character();
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
		int skillLevel(std::string skillName);
	protected:
		int mStrength;
		int mDexterity;
		int mConstitution;
		int mIntelligence;
		int mWisdom;
		int mCharisma;


		std::map<std::string, int> mSkillLevels;
};

#endif // CHARACTER_H
