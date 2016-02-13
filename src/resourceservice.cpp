#include "resourceservice.h"
#include <json/json.h>
#include <fstream>
#include <cassert>
#include "item.h"
#include "character.h"
#include "player.h"
#include "util/textutils.h"
#include <boost/filesystem.hpp>
#include "defines.h"
#include <boost/thread/lock_guard.hpp>

namespace fs = boost::filesystem;


ResourceService *RS = 0;

ResourceService::ResourceService() {
    assert(RS == 0);
    RS = this;
}

ResourceService::~ResourceService() {
    RS = 0;
}

ResourceService *ResourceService::instance() {
    assert(RS);
    return RS;
}

Json::Value ResourceService::readJsonFile(const std::string &path) const {
    Json::Value ret;
    std::ifstream file(path);
    if (!file) {
        throw SerializationException("Failed to open file " + path);
        return Json::Value();
    }
    Json::Reader reader;

    if (!reader.parse(file, ret, false)) {
        throw SerializationException(path + ": Json: " + reader.getFormattedErrorMessages());
    }

    return ret;
}

bool ResourceService::saveJsonFile(const std::string &path, const Json::Value &val) const {
    std::ofstream file(path);
    if (!file) {
        throw SerializationException("Failed to write file " + path);
        return false;
    }

    Json::StyledStreamWriter writer;
    writer.write(file, val);
    return true;
}

std::unique_ptr<Item> ResourceService::item(const std::string &id) {
    std::shared_ptr<Item> base = baseItem(id);
    if (!base) return std::unique_ptr<Item>();

    std::unique_ptr<Item> i = base->clone();
    return std::move(i);
}

std::shared_ptr<Item> ResourceService::baseItem(const std::string &id) {
    boost::lock_guard<boost::recursive_mutex> lock(mItemMutex);

    auto itemIt = mBaseItems.find(id);
    if (itemIt != mBaseItems.end()) return itemIt->second;
    return nullptr;
}

std::unique_ptr<Item> ResourceService::itemCopyForEditing(const std::string &id)
{
    std::shared_ptr<Item> base = baseItem(id);
    if (!base) return std::unique_ptr<Item>();

    std::unique_ptr<Item> ret(new Item(base->id()));
    base->clone(*ret);
    return std::move(ret);
}

void ResourceService::storeItem(std::unique_ptr<Item> &&item)
{
    boost::lock_guard<boost::recursive_mutex> lock(mItemMutex);
    std::shared_ptr<Item> base = baseItem(item->id());
    if (!base) {
        base = std::shared_ptr<Item>(item.release());
        mBaseItems[base->id()] = base;
    }
    else {
        item->clone(*base);
    }
    saveItem(base);
}

bool ResourceService::saveItem(const std::shared_ptr<Item> &item)
{
    boost::lock_guard<boost::recursive_mutex> lock(mItemMutex);
    std::string path = item->id();
    if (path.empty()) {
        std::cerr << "Empty item id. Save failed." << std::endl;
        return;
    }

    path += ".item";

    Json::Value val = Json::serialize(item);

    //Level item template
    if (path[0] == '$') {
        path = std::string("data/levels/") + (path.c_str() + 1);
    }
    else {
        path = "data/items/" + path;
    }
    try {
        saveJsonFile(path, val);
        return true;
    }
    catch (const SerializationException &e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

bool ResourceService::loadAllItemTemplates()
{
    boost::lock_guard<boost::recursive_mutex> lock(mItemMutex);

    bool success = true;
    success &= loadItemsFromDirectory("data/items", "");
    success &= loadItemsFromDirectory("data/levels", "$");

    return success;
}

bool ResourceService::loadItemsFromDirectory(const std::string &path, const std::string &prefix)
{
    fs::path loadPath(path);
    fs::directory_iterator endIt;

    if (!fs::exists(loadPath) || !fs::is_directory(loadPath)) return true;

    bool success = true;

    for (fs::directory_iterator dirIt(loadPath); dirIt != endIt; ++dirIt) {
        fs::path path = *dirIt;
        if (fs::is_directory(path)) {
            success &= loadItemsFromDirectory(path.string(), prefix + path.stem().string() + "/");
        }
        else if (fs::is_regular_file(path) && path.extension().string() == ".item") {
            success &= loadItem(path.string(), prefix + path.stem().string());
        }
    }
    return success;
}

bool ResourceService::loadItem(const std::string &path, const std::string &id)
{
    try {
        Json::Value v = readJsonFile(path);
        std::shared_ptr<Item> item = std::make_shared<Item>(id);
        item->deserialize(v);
        mBaseItems[id] = item;
        return true;
    }
    catch (const SerializationException &e) {
        std::cerr << "Failed to load item " << path << std::endl;
        std::cerr << e.what() << std::endl;
        return false;
    }
}


