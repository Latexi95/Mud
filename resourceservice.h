#ifndef RESOURCESERVICE_H
#define RESOURCESERVICE_H
#include <json/value.h>
#include <memory>
#include <boost/thread/mutex.hpp>
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

		std::unique_ptr<Item> item(const std::string &path);
		std::shared_ptr<Item> baseItem(const std::string &path);
	private:
		std::vector<std::shared_ptr<Player>> mPlayer;
		std::unordered_map<std::string, std::shared_ptr<Item> > mBaseItems;
		boost::mutex mMutex;
};

extern ResourceService *RS;

#endif // RESOURCESERVICE_H
