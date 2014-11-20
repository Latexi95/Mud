#include "player.h"

Player::Player() {

}

Player::Player(const std::string &name) :
	mName(name)
{
}

void Player::setName(const std::string &name) {
	mName = name;
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
	return val;
}

bool Player::deserialize(const Json::Value &val) {
	const Json::Value &name = val["name"];
	if (name.isString())
		mName = name.asString();
	else
		return false;
	const Json::Value &hash = val["password-hash"];
	if (name.isString())
		mPasswordHash = hash.asString();
	else
		return false;
	return true;
}

bool Player::isLoggedIn() const {
	return mLoggedIn;
}

void Player::setLoggedIn(bool loggedIn) {
	return mLoggedIn;
}

std::string Player::hashPassword(const std::string &password) const {
	//TODO: hash
	return password;
}
