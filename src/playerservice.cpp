#include "playerservice.h"
#include "resourceservice.h"
#include <cassert>
#include "mud.h"

static PlayerService *sInstance = 0;

PlayerService::PlayerService(Mud *mud) :
    mMud(mud) {
    assert(!sInstance);
    sInstance = this;
}

PlayerService::~PlayerService() {
    sInstance = 0;
}

PlayerService *PlayerService::instance() {
    assert(sInstance);
    return sInstance;
}

std::shared_ptr<Player> PlayerService::findPlayerByName(const std::string &name) {
    boost::unique_lock<boost::mutex> lock(mMutex);
    for (const std::shared_ptr<Player> &p : mPlayers) {
        if (p->name() == name) {
            return p;
        }
    }

    try {
        Json::Value val = ResourceService::instance()->readJsonFile("data/players.json");
        if (!val.isObject()) {
            return nullptr;
        }
        Json::Value pval = val[name];
        if (!pval.isObject()) return nullptr;

        std::shared_ptr<Player> p = std::make_shared<Player>(name);
        Json::deserialize(pval, p);
        mPlayers.push_back(p);
        return p;
    }
    catch (const SerializationException &e) {
        std::cerr << e.what();
        return nullptr;
    }
}

std::shared_ptr<Player> PlayerService::createPlayer(const std::string &name) {
    boost::unique_lock<boost::mutex> lock(mMutex);
    std::shared_ptr<Player> p = std::make_shared<Player>(name);
    mPlayers.push_back(p);
    return p;
}

void PlayerService::savePlayers() {
    boost::unique_lock<boost::mutex> lock(mMutex);
    Json::Value val = ResourceService::instance()->readJsonFile("data/players.json");
    if (!val.isObject()) {
        val = Json::Value(Json::objectValue);
    }

    for (const std::shared_ptr<Player> &p : mPlayers) {
        if (p->isComplete()) {
            val[p->name()] = p->serialize();
        }
    }

    ResourceService::instance()->saveJsonFile("data/players.json", val);
}
