#include "skillcheck.h"
#include "dicerandom.h"
#include "character.h"
#include <algorithm>

SkillCheck::SkillCheck(const std::string &skillName, int difficulty) :
	mSkillName(skillName),
	mDifficulty(difficulty) {
	std::transform(mSkillName.begin(), mSkillName.end(), mSkillName.begin(), ::tolower);
}

SkillCheck::~SkillCheck() {

}

bool SkillCheck::check(Character *character, DiceRandom &random) {
	int mod = 0;
	if (mSkillName == "strength" || mSkillName == "str") {
		mod = character->STR();
	} else if (mSkillName == "dexterity" || mSkillName == "dex") {
		mod = character->DEX();
	} else if (mSkillName == "charisma" || mSkillName == "cha") {
		mod = character->CHA();
	} else if (mSkillName == "constitution" || mSkillName == "con") {
		mod = character->CON();
	} else if (mSkillName == "intelligence" || mSkillName == "int") {
		mod = character->INT();
	} else if (mSkillName == "wisdom" || mSkillName == "wis") {
		mod = character->WIS();
	} else {
		mod = character->skillLevel(mSkillName);
	}

	return random.roll(20) - mod >= mDifficulty;
}
