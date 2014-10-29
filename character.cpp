#include "character.h"
#include <algorithm>
#include "resourceservice.h"

Character::Character() {

}

Character::Character(const std::string &name) :
	mName(name)
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
	for (const std::pair<std::string, std::vector<RHandle<Item> > > &equipmentSlot : mEquipment) {
		Json::Value items(Json::arrayValue);
		for (const RHandle<Item> &i : equipmentSlot.second) {
			if (i.isDynamicResource()) {
				items.append(i->serialize());
			}
			else {
				items.append(i.path());
			}
		}
		equipment[equipmentSlot.first] = items;
	}
	obj["equipment"] = equipment;
	return obj;
}

bool Character::deserialize(const Json::Value &val) {
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
				RHandle<Item> item = ResourceService::instance()->item(equipmentSlot->asString());
				if (!item.isNull()) {
					mEquipment[equipmentSlot.memberName()].push_back(item);
				}
			}
			else if (equipmentSlot->isObject()) {
				RHandle<Item> item = createDynamicResource<Item>();
				if (!item->deserialize(*equipmentSlot)) {
					continue;
				}
				mEquipment[equipmentSlot.memberName()].push_back(item);
			} else if (equipmentSlot->isArray()) {
				std::vector<RHandle<Item>> &items = mEquipment[equipmentSlot.memberName()];
				for (Json::ValueConstIterator i2 = equipmentSlot->begin(); i2 != equipmentSlot->end(); i2++) {
					if (i2->isString()) {
						RHandle<Item> item = ResourceService::instance()->item(i2->asString());
						if (!item.isNull()) {
							items.push_back(item);
						}
					}
					else if (equipmentSlot->isObject()) {
						RHandle<Item> item = createDynamicResource<Item>();
						if (!item->deserialize(*i2)) {
							continue;
						}
						items.push_back(item);
					}
				}
			}
		}
	}

	return true;
}
