#include "characterservice.h"
#include "resourceservice.h"

CharacterService *CS = 0;

CharacterService::CharacterService() {
	assert(CS == 0);
	CS = this;
}

CharacterService::~CharacterService() {
	assert(CS);
	CS = 0;
}

CharacterService *CharacterService::instance() {
	assert(CS);
	return CS;
}

std::shared_ptr<Character> CharacterService::character(const std::string &characterId) {
	boost::unique_lock<boost::mutex> lock(mMutex);
	auto characterIt = mCharacters.find(characterId);
	if (characterIt != mCharacters.end()) return characterIt->second;

	Json::Value characterJson = RS->readJsonFile("data/characters/" + characterId + ".json");

	std::shared_ptr<Character> character = std::make_shared<Character>(characterId);
	character->deserialize(characterJson);
	mCharacters[characterId] = character;
	return character;
}

std::shared_ptr<Character> CharacterService::createCharacter(const std::string &characterId) {
	boost::unique_lock<boost::mutex> lock(mMutex);
	std::shared_ptr<Character> character = std::make_shared<Character>(characterId);
	mCharacters[characterId] = character;
	return character;
}

void CharacterService::saveCharacter(const std::shared_ptr<Character> &c) {
	Json::Value v = c->serialize();
	RS->saveJsonFile("data/characters/" + c->name() + ".json", v);
}
