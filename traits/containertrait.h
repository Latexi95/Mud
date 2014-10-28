#ifndef CONTAINERTRAIT_H
#define CONTAINERTRAIT_H
#include "itemtrait.h"
#include "resource.h"
#include "item.h"
#include <vector>
class ContainerTrait : public ItemTrait  {
	public:
		enum ContainerType {
			Open,
			Closed,
			Opened
		};

		ContainerTrait();
		~ContainerTrait();
		Type type() const { return Container; }

		ContainerType containerType() const { return mContainerType; }
		void setContainerType(ContainerType t) { mContainerType = t; }
		const std::vector<RHandle<Item> > containedItems() const { return mContainedItems; }

		Json::Value serialize() const;
		bool deserialize(const Json::Value &val);
		bool hasToBeSerialized(const ItemTrait *base) const;


	private:
		std::vector<RHandle<Item> > mContainedItems;
		ContainerType mContainerType;
};

#endif // CONTAINERTRAIT_H
