#ifndef CONTAINERTRAIT_H
#define CONTAINERTRAIT_H
#include "itemtrait.h"
#include "item.h"
#include <vector>
#include <boost/algorithm/string.hpp>
#include "util/enums.h"

class ContainerTrait : public ItemTrait  {
public:

    static const ItemTraitType staticTraitType = ItemTraitType::Container;

    ContainerTrait();
    virtual ~ContainerTrait();
    ItemTraitType type() const { return ItemTraitType::Container; }


    ContainerState containerType() const { return mContainerType; }
    void setContainerType(ContainerState t) { mContainerType = t; }
    const std::vector<std::unique_ptr<Item> > &containedItems() const { return mContainedItems; }

    std::unique_ptr<ItemTrait> clone() const;
    virtual Json::Value serialize() const;
    virtual void deserialize(const Json::Value &val);

    virtual void accept(ItemTraitVisitor *visitor);
    const char *traitName() const { return "container"; }
private:
    std::vector<std::unique_ptr<Item>> mContainedItems;
    ContainerState mContainerType;
};

namespace Json {
template<>
struct Serializer<ContainerState> {
    static Json::Value serialize(ContainerState t){
        switch (t) {
        case ContainerState::Open:
            return "open";
        case ContainerState::Closed:
            return "closed";
        case ContainerState::Opened:
            return "opened";
        default:
            return "invalid";
        }
    }

    static void deserialize(const Json::Value &val, ContainerState &t) {
        if (!val.isString()) throw SerializationException("Expecting a string to deserialize ContainerTrait::ContainerType");
        std::string s = val.asString();
        boost::algorithm::to_lower(s);
        if (s == "open") {
            t = ContainerState::Open;
        } else if (s == "closed") {
            t = ContainerState::Closed;
        } else if (s == "opened") {
            t = ContainerState::Opened;
        } else {
            throw SerializationException("No ContainerTrait::ContainerType value named \"" + s + '"');
        }
    }
};


}

#endif // CONTAINERTRAIT_H
