#ifndef RESOURCESERVICE_H
#define RESOURCESERVICE_H
#include <json/value.h>
#include <boost/atomic.hpp>
#include "resource.h"

class ResourceService {
	public:
		ResourceService();
		~ResourceService();
		Json::Value requestJsonResource(const std::string &path) const;
		RHandle<Item> item(const std::string &path);
	private:
		ResourceStash<Item> mItemStash;
};

#endif // RESOURCESERVICE_H
