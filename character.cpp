#include "character.h"
#include <algorithm>

Character::Character(const std::string &name) :
	Item(name)
{
}

Character::~Character() {

}

void Character::setSkillLevel(std::string skillName, int level) {
	std::transform(skillName.begin(), skillName.end(), skillName.begin(), ::tolower);
	mSkillLevels[skillName] = level;
}

int Character::skillLevel(std::string skillName) {
	std::transform(skillName.begin(), skillName.end(), skillName.begin(), ::tolower);
	std::map<std::string, int>::const_iterator i = mSkillLevels.find(skillName);
	if (i != mSkillLevels.end()) {
		return i->second;
	}
	return 0;
}
