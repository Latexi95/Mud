#include "levelservice.h"
#include "resourceservice.h"
#include "maineventqueue.h"
#include "events/event.h"
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

LevelService *LS = 0;

LevelService::LevelService() {
    assert(LS == 0);
    LS = this;
}

LevelService::~LevelService() {
    assert(LS);
    LS = 0;
}

LevelService *LevelService::instance() {
    assert(LS);
    return LS;
}

Level *LevelService::level(const std::string &levelId) {
    boost::unique_lock<boost::mutex> lock(mMutex);
    auto levelIt = mLevels.find(levelId);
    if (levelIt != mLevels.end()) return levelIt->second.get();
    return nullptr;
}

std::shared_ptr<Level> LevelService::loadLevel(const std::string &levelId, bool directAdd)
{
    boost::unique_lock<boost::mutex> lock(mMutex);
    return loadLevelNoLock(levelId, directAdd);
}

void LevelService::saveAllLevels()
{
    boost::unique_lock<boost::mutex> lock(mMutex);
    for (auto &levelPair : mLevels) {
        const std::shared_ptr<Level> &level = levelPair.second;
        std::string path = "data/levels/" + level->id() + "/";
        try {
            level->forEachRoom([&](Room *r) {
                try {
                    Json::Value val = Json::serialize(*r);
                    RS->saveJsonFile(path + r->id() + ".room", val);
                }
                catch (const SerializationException &e) {
                    std::cerr << "Failed saving room " << r->id() << ": " << e.what() << std::endl;
                }
            });
            Json::Value val = Json::serialize(level);
            RS->saveJsonFile(path + level->id() + ".level", val);
        }
        catch (const SerializationException &e) {
            std::cerr << "Failed saving level " << level->id() << ": " << e.what() << std::endl;
        }
    }
}

std::shared_ptr<Level> LevelService::loadLevelNoLock(const std::string &levelId, bool directAdd)
{
    Json::Value levelJson = RS->readJsonFile("data/levels/" + levelId + "/" + levelId + ".level");

    std::shared_ptr<Level> level = std::make_shared<Level>(levelId);
    Json::deserialize(levelJson, level);

    if (mLevels.find(level->id()) != mLevels.end()) {
        std::cerr << "Tried to load level with already existing id \"" << level->id() << "\"again!";
        return mLevels.find(level->id())->second;
    }

    for (const std::string &roomId : level->roomIds()) {
        try {
        std::unique_ptr<Room> room(new Room(roomId));
        Json::Value val = RS->readJsonFile("data/levels/" + levelId + "/" + roomId + ".room");
        Json::deserialize(val, room);
        level->addRoom(std::move(room));
        }
        catch (const SerializationException &e) {
            std::cerr << "Loading room " << roomId << " failed: " << e.what() << std::endl;
            throw;
        }
    }

    mLevels[level->id()] = level;

    if (directAdd) {
        MEQ->addLevelEventQueue(level->eventQueue());
    }
    else {
        MEQ->push(makeFunctionEvent([level](Event *e) {
                      MEQ->addLevelEventQueue(level->eventQueue());
                  }));
    }
    return level;
}

bool LevelService::loadAllLevels()
{
    boost::unique_lock<boost::mutex> lock(mMutex);
    fs::path loadPath("data/levels");
    fs::directory_iterator endIt;

    if (!fs::exists(loadPath) || !fs::is_directory(loadPath)) return false;

    for (fs::directory_iterator dirIt(loadPath); dirIt != endIt; ++dirIt) {
        fs::path dirPath = *dirIt;
        if (fs::is_directory(dirPath)) {
            std::string levelId = dirPath.filename().string();
            fs::path levelFilePath = dirPath / (levelId + ".level");
            if (fs::exists(levelFilePath) && fs::is_regular_file(levelFilePath)) {
                try {
                    loadLevelNoLock(levelId, true);
                }
                catch (const SerializationException &e) {
                    std::cerr << "Loading " << levelId << " failed\n";
                    std::cerr << e.what() << std::endl;
                }
            }
        }
    }

    for (auto &idLevelPair : mLevels) {
        idLevelPair.second->resolveRoomExits();
    }

    return true;
}
