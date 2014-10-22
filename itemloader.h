#ifndef ITEMLOADER_H
#define ITEMLOADER_H
#include "resource.h"
#include "resourceservice.h"
#include "item.h"
class ItemLoader {
	public:
		ItemLoader(ResourceService *resServ);
		~ItemLoader();

		Resource<Item> *operator()(const std::string &filePath);
	private:
		ResourceService *mResourceService;
		bool checkIsObject(const Json::Value &val, const char *name, const std::string &filePath) const;
};

#endif // ITEMLOADER_H
