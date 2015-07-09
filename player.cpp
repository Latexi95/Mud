#include "player.h"

Player::Player() :
    mLoggedIn(false) {

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
    val["characters"] = Json::serialize(mCharacterNames);
    return val;
}

void Player::deserialize(const Json::Value &val) {
    Json::deserialize(val["name"], mName);
    Json::deserialize(val["password-hash"], mPasswordHash);
    Json::deserialize(val["characters"], mCharacterNames);
}

bool Player::isLoggedIn() const {
    return mLoggedIn;
}

void Player::setLoggedIn(bool loggedIn) {
    mLoggedIn = loggedIn;
}

bool Player::isComplete() const
{
    return !mName.empty() && !mPasswordHash.empty();
}

const std::vector<std::string> &Player::characterNames() const {
    return mCharacterNames;
}

void Player::addCharacterName(const std::string &name) {
    mCharacterNames.push_back(name);
}

std::shared_ptr<Character> Player::currentCharacter() const {
    return mCurrentCharacter;
}

std::string Player::hashPassword(const std::string &password) const {
    //TODO: hash
    return password;
}
