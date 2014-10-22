#include "player.h"

Player::Player()
{
}

bool Player::passwordMatch(const std::string &password) const {
	return hashPassword(password) == mPasswordHash;
}

void Player::setPassword(const std::string &password) {
	mPasswordHash = hashPassword(password);
}

Json::Value Player::serialize() const {
	Json::Value val(Json::objectValue);
	val["name"] = mName;
	val["password-hash"] = mPasswordHash;
}

bool Player::deserialize(const Json::Value &val) {
	bool success = true;
	mName = valueAsString(val["name"]);
	mPasswordHash = valueAsString(val["password-hash"]);
	return success;
}

std::string Player::hashPassword(const std::string &password) {
	//TODO: hash
	return password;
}
