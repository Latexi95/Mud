#ifndef RESOURCESERVICE_H
#define RESOURCESERVICE_H
#include <json/value.h>
#include <memory>
#include <boost/thread/recursive_mutex.hpp>
#include <unordered_map>
#include <utility>
#include <memory>

class Item;
class Character;
class Player;
class ResourceService {
public:
    ResourceService();
    ~ResourceService();
    static ResourceService *instance();

    Json::Value readJsonFile(const std::string &path) const;
    bool saveJsonFile(const std::string &path, const Json::Value &val) const;

    std::unique_ptr<Item> item(const std::string &id);
    std::shared_ptr<Item> baseItem(const std::string &id);

    std::unique_ptr<Item> itemCopyForEditing(const std::string &id);
    void storeItem(std::unique_ptr<Item> &&item);

    bool saveItem(const std::shared_ptr<Item> &item);
    bool loadAllItemTemplates();
private:
    bool loadItemsFromDirectory(const std::string &path, const std::string &prefix);
    bool loadItem(const std::string &path, const std::string &id);

    std::unordered_map<std::string, std::shared_ptr<Item> > mBaseItems;

    boost::recursive_mutex mItemMutex;
};

extern ResourceService *RS;

#endif // RESOURCESERVICE_H
