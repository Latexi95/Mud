#ifndef RESOURCESERVICE_H
#define RESOURCESERVICE_H
#include <json/value.h>
#include <memory>
#include <boost/thread/recursive_mutex.hpp>
#include <unordered_map>
#include <utility>
#include <memory>

class Item;
class BaseItem;
class Character;
class Player;
class ResourceService {
public:
    ResourceService();
    ~ResourceService();
    static ResourceService *instance();

    Json::Value readJsonFile(const std::string &path) const;
    bool saveJsonFile(const std::string &path, const Json::Value &val) const;

    std::unique_ptr<Item> createItem(const std::string &id);
    std::shared_ptr<BaseItem> baseItem(const std::string &id);

    std::unique_ptr<BaseItem> baseItemCopyForEditing(const std::string &id);
    void storeBaseItem(std::unique_ptr<BaseItem> &&item);

    bool saveBaseItem(const std::shared_ptr<BaseItem> &item);
    bool loadAllItemTemplates();
private:
    bool loadItemsFromDirectory(const std::string &path, const std::string &prefix);
    bool loadItem(const std::string &path, const std::string &id);

    std::unordered_map<std::string, std::shared_ptr<BaseItem> > mBaseItems;

    boost::recursive_mutex mItemMutex;
};

extern ResourceService *RS;

#endif // RESOURCESERVICE_H
