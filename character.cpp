#include "character.h"
#include <algorithm>
#include "resourceservice.h"

Character::Character() :
	mGender(Male),
	mAge(25),
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
Character::Character(const std::string &name, Gender gender) :
	mGender(gender),
	mName(name),
	mAge(25),
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
	std::map<std::string, int>::const_iterator i = mSkillLevels.find(skillName);
	if (i != mSkillLevels.end()) {
		return i->second;
	}
	return 0;
}

Json::Value Character::serialize() const {
	Json::Value obj(Json::objectValue);
	obj["str"] = mStrength;
	obj["dex"] = mDexterity;
	obj["con"] = mConstitution;
	obj["int"] = mIntelligence;
	obj["wis"] = mWisdom;
	obj["cha"] = mCharisma;
	obj["height"] = mHeight;
	obj["width"] = mWidth;

	Json::Value skills(Json::objectValue);
	for (const std::pair<std::string, int> &p : mSkillLevels) {
		skills[p.first] = p.second;
	}
	obj["skills"] = skills;
	Json::Value equipment(Json::objectValue);
	for (auto it = mEquipment.begin(); it != mEquipment.end(); ++it) {
		Json::Value items(Json::arrayValue);
		for (const std::unique_ptr<Item> &i : it->second) {
			items.append(i->serialize());
		}
		equipment[it->first] = items;
	}
	obj["equipment"] = equipment;
	return obj;
}

void Character::deserialize(const Json::Value &val) {
	const Json::Value &str = val["str"];
	mStrength = str.isInt() ? str.asInt() : 0;
	const Json::Value &dex = val["dex"];
	mDexterity = dex.isInt() ? dex.asInt() : 0;
	const Json::Value &con = val["con"];
	mConstitution = con.isInt() ? con.asInt() : 0;
	const Json::Value &int_ = val["int"];
	mIntelligence = int_.isInt() ? int_.asInt() : 0;
	const Json::Value &wis = val["wis"];
	mWisdom = wis.isInt() ? wis.asInt() : 0;
	const Json::Value &cha = val["cha"];
	mCharisma = cha.isInt() ? cha.asInt() : 0;

	const Json::Value &height = val["height"];
	mHeight = height.isInt() ? height.asInt() : 0;
	const Json::Value &width = val["width"];
	mWidth = width.isInt() ? width.asInt() : 0;

	const Json::Value &skills = val["skills"];
	if (skills.isObject()) {
		for (Json::ValueConstIterator i = skills.begin(); i != skills.end(); i++) {
			if (i->isInt()) {
				mSkillLevels[i.memberName()] = i->asInt();
			}
		}
	}

	const Json::Value &equipment = val["equipment"];
	if (equipment.isObject()) {
		for (Json::ValueConstIterator equipmentSlot = equipment.begin(); equipmentSlot != equipment.end(); equipmentSlot++) {
			if (equipmentSlot->isString()) {
				std::unique_ptr<Item> item = ResourceService::instance()->item(equipmentSlot->asString());
				if (item) {
					mEquipment[equipmentSlot.memberName()].emplace_back(std::move(item));
				}
			}
			else if (equipmentSlot->isObject()) {
				std::unique_ptr<Item> item = std::unique_ptr<Item>(new Item());
				item->deserialize(*equipmentSlot);
				mEquipment[equipmentSlot.memberName()].emplace_back(std::move(item));
			} else if (equipmentSlot->isArray()) {
				std::vector<std::unique_ptr<Item>> &items = mEquipment[equipmentSlot.memberName()];
				for (Json::ValueConstIterator i2 = equipmentSlot->begin(); i2 != equipmentSlot->end(); i2++) {
					if (i2->isString()) {
						std::unique_ptr<Item> item = ResourceService::instance()->item(i2->asString());
						if (item) {
							items.emplace_back(std::move(item));
						}
					}
					else if (equipmentSlot->isObject()) {
						std::unique_ptr<Item> item = std::unique_ptr<Item>(new Item());
						item->deserialize(*i2);
						items.emplace_back(std::move(item));
					}
					else {
						assert("Invalid item json" && 0);
					}
				}
			} else {
				assert("Invalid item json" && 0);
			}
		}
	}
}
