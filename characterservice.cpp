#include "characterservice.h"
#include "resourceservice.h"
#include <boost/filesystem.hpp>
#include "room.h"
#include "level.h"
#include "events/moveevent.h"
#include "mud.h"
#include "maineventqueue.h"

namespace fs = boost::filesystem;
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
    UNIQUE_LOCK(mMutex);
    auto characterIt = mCharacters.find(characterId);
    if (characterIt != mCharacters.end()) return characterIt->second;
    return nullptr;
}

std::shared_ptr<Character> CharacterService::createCharacter(const std::string &characterId) {
    UNIQUE_LOCK(mMutex);
    std::shared_ptr<Character> character = std::make_shared<Character>(characterId);
    character->setSaveWithLevel(false);
    mCharacters[characterId] = character;
    return character;
}

bool CharacterService::doesCharacterExits(const std::string &characterId)
{
    UNIQUE_LOCK(mMutex);
    return mCharacters.find(characterId) != mCharacters.end();
}

void CharacterService::saveCharacter(const std::shared_ptr<Character> &c) {
    Json::Value v = Json::serialize(c);
    RS->saveJsonFile("data/characters/" + c->id() + ".json", v);
}

bool CharacterService::canMove(const std::shared_ptr<Character> &c, Direction dir)
{
    Room *r = c->room();
    const Wall &w = r->wall(dir);
    return w.hasExit();
}

void CharacterService::startMove(const std::shared_ptr<Character> &c, Direction dir)
{
    Level *currentLevel = c->level();
    Room *r = c->room();
    RoomExit *exit = r->exit(dir);
    if (exit) {
        MoveStartEvent(c, exit->targetRoom()).execute();

        Level *targetLevel = exit->targetRoom()->level();
        auto moveEndEvent = new MoveEndEvent(c, exit->targetRoom());
        auto moveEndTime = MUD->timeAfter(travelTime(c, exit->travelDistance()));
        if (currentLevel == targetLevel) {
            currentLevel->eventQueue()->push(moveEndTime, moveEndEvent);
        }
        else {
            MEQ->push(moveEndTime, moveEndEvent);
        }
    }

}

float CharacterService::travelSpeed(const std::shared_ptr<Character> &)
{
    return 2;
}

float CharacterService::travelTime(const std::shared_ptr<Character> &r, float distance)
{
    return distance / travelSpeed(r);
}

bool CharacterService::loadAllCharacters()
{
    UNIQUE_LOCK(mMutex);
    fs::path loadPath("data/characters");
    fs::directory_iterator endIt;

    if (!fs::exists(loadPath) || !fs::is_directory(loadPath)) return false;

    for (fs::directory_iterator dirIt(loadPath); dirIt != endIt; ++dirIt) {
        fs::path filePath = *dirIt;
        if (fs::is_regular_file(filePath)) {
            if (filePath.extension().string() == ".json") {
                try {
                    Json::Value characterJson = RS->readJsonFile(filePath.string());
                    std::shared_ptr<Character> character = std::make_shared<Character>();
                    Json::deserialize(characterJson, character);
                    if (mCharacters[character->id()]) {
                        std::cerr << "Duplicated character id " << character->id() << std::endl;
                        std::cerr << "Ignored";
                        continue;
                    }
                    mCharacters[character->id()] = character;
                }
                catch (const SerializationException &e) {
                    std::cerr << "Loading " << filePath.string() << " failed" << std::endl;
                    std::cerr << e.what();
                }
            }
        }
    }
    return true;
}

std::shared_ptr<Character> CharacterService::loadCharacter(const std::string &fileName)
{
    UNIQUE_LOCK(mMutex);

    Json::Value characterJson = RS->readJsonFile("data/characters/" + fileName);

    std::shared_ptr<Character> character = std::make_shared<Character>();
    try {
        Json::deserialize(characterJson, character);
        if (mCharacters[character->id()]) {
            std::cerr << "Duplicated character id " << character->id() << std::endl;
            return mCharacters[character->id()];
        }
        mCharacters[character->id()] = character;
        return character;
    }
    catch (const SerializationException &e) {
        std::cerr << "Loading " << fileName << " failed" << std::endl;
        std::cerr << e.what();
    }
    return nullptr;
}

std::vector<Item *> CharacterService::itemsInVision(const std::shared_ptr<Character> &c)
{
    //TODO: Implement vision detection
    std::vector<Item *> items;
    Room *r = c->room();
    for (const Wall &w : r->walls()) {
        for (auto &item : w.items()) {
            items.push_back(item.get());
        }
    }
    return items;
}
