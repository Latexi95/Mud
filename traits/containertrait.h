#ifndef CONTAINERTRAIT_H
#define CONTAINERTRAIT_H
#include "itemtrait.h"
#include "item.h"
#include <vector>
#include <boost/algorithm/string.hpp>

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
		const std::vector<std::unique_ptr<Item> > &containedItems() const { return mContainedItems; }

		std::unique_ptr<ItemTrait> clone() const;
		Json::Value serialize() const;
		void deserialize(const Json::Value &val);

		const char *traitName() const { return "container"; }
	private:
		std::vector<std::unique_ptr<Item>> mContainedItems;
		ContainerType mContainerType;
};

namespace Json {
template<>
struct Serializer<ContainerTrait::ContainerType> {
		static Json::Value serialize(ContainerTrait::ContainerType t){
			switch (t) {
				case ContainerTrait::Open:
					return "open";
				case ContainerTrait::Closed:
					return "closed";
				case ContainerTrait::Opened:
					return "opened";
			}
		}

		static void deserialize(const Json::Value &val, ContainerTrait::ContainerType &t) {
			if (val.isString()) throw SerialiazationException("Expecting a string to deserialize ContainerTrait::ContainerType");
			std::string s = val.asString();
			boost::algorithm::to_lower(s);
			if (s == "open") {
				t = ContainerTrait::Open;
			} else if (s == "closed") {
				t = ContainerTrait::Closed;
			} else if (s == "opened") {
				t = ContainerTrait::Opened;
			} else {
				throw SerialiazationException("No ContainerTrait::ContainerType value named \"" + s + '"');
			}
		}
};


}

#endif // CONTAINERTRAIT_H
