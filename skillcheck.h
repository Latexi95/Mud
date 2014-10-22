#ifndef SKILLCHECK_H
#define SKILLCHECK_H
#include <string>
class DiceRandom;
class Character;
class SkillCheck {
	public:
		SkillCheck(const std::string &skillName, int difficulty);
		~SkillCheck();
		bool check(Character *character, DiceRandom &random);
	private:
		std::string mSkillName;
		int mDifficulty;
};

#endif // SKILLCHECK_H
