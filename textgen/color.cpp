#include "color.h"
#include <unordered_map>
#include "resourceservice.h"
#include <list>
#include <boost/algorithm/string.hpp>
#include "textutils.h"
using namespace text;
std::list<Color> sColors;
std::unordered_map<std::string, Color*> sNameMap;



text::Color *text::colorByName(const std::string &name) {
	std::string n = cleaned(name);
	boost::algorithm::to_lower(n);
	auto it = sNameMap.find(n);
	if (it != sNameMap.end()) return it->second;
	return nullptr;
}


void Color::loadColors() {
	sColors.clear();
	sNameMap.clear();
	Json::Value v = RS->readJsonFile("data/colors.json");
	for (Json::ValueIterator i = v.begin(); i != v.end(); ++i) {
		sColors.emplace_back();
		Color &color = sColors.back();
		color.mName = i.memberName();
		sNameMap[color.mName] = &color;
		assert(i->isObject());
		Json::Value alts = i->get("alts", Json::Value());
		if (alts.isArray()) {
			for (Json::ValueIterator altIt = alts.begin(); altIt != alts.end(); ++altIt) {
				if (altIt->isString()) {
					std::string s = altIt->asCString();
					color.mAltNames.push_back(s);
					sNameMap[s] = &color;
				}
			}
		}
		else if (alts.isString()) {
			std::string s = alts.asCString();
			color.mAltNames.push_back(s);
			sNameMap[s] = &color;
		}
		Json::Value desc = i->get("description", Json::Value());
		if (desc.isString()) color.mDescription = desc.asCString();

		Json::Value rgb = i->get("rgb", Json::Value());
		if (desc.isString()) {
			std::string rgbStr = rgb.asCString();
			int colorRgb = 0;
			if (rgbStr.length() == 6) {
				for (int ci = 0; ci < 6; ++ci) {
					char c = std::tolower(rgbStr[ 5 - ci]);
					if (c >= '0' && c <= '9') {
						colorRgb += (c - '0') << (4 * ci);
					}
					else if (c >= 'a' && c <= 'f') {
						colorRgb += (c - 'a' + 10) << (4 * ci);
					}
				}
			}
			color.mRGB = colorRgb;
		}
	}
}

void Color::saveColors() {

	Json::Value root(Json::objectValue);

	for (const Color &c : sColors) {
		Json::Value cobj(Json::objectValue);
		if (c.mAltNames.size() == 1) {
			cobj["alts"] = c.mAltNames.front();
		} else if (c.mAltNames.size() > 1) {
			Json::Value altArray(Json::arrayValue);
			for (const std::string &s : c.mAltNames) {
				altArray.append(s);
			}
			cobj["alts"] = altArray;
		}
		cobj["rgb"] = c.mRGB;
		cobj["description"] = c.mDescription;
		root[c.mName] = cobj;
	}

	RS->saveJsonFile("data/colors.json", root);
}
