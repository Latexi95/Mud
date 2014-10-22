#include "jsonserializable.h"
JsonSerializable::JsonSerializable() {
}

JsonSerializable::~JsonSerializable() {

}

std::string JsonSerializable::valueAsString(const Json::Value &val, bool &success) {
	if (val.isString()) return val.asString();
	success = false;
	return "";
}
