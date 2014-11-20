#ifndef RESOURCESERVICE_H
#define RESOURCESERVICE_H
#include <json/value.h>
#include <boost/atomic.hpp>
#include "resource.h"
class Item;
class Character;
class ResourceService {
	public:
		ResourceService();
		~ResourceService();
		static ResourceService *instance();

		Json::Value requestJsonResource(const std::string &path) const;
		RHandle<Item> item(const std::string &path);
		RHandle<Character> character(const std::string &path);
		RHandle<Player> player(const std::string &name);
		RHandle<Player> createPlayer(const std::string &name);

		void save(RHandle<Player> player);

	private:
		ResourceStash<Item> mItemStash;
		ResourceStash<Character> mCharacterStash;
		ResourceStash<Player> mPlayerStash;
};

#endif // RESOURCESERVICE_H
