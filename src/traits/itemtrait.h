#ifndef ITEMTRAIT_H
#define ITEMTRAIT_H
#include "jsonserializable.h"
#include <memory>
#include <boost/algorithm/string.hpp>
#include "util/enums.h"

class ItemTrait {
public:
    ItemTrait();
    virtual ~ItemTrait();
    virtual std::unique_ptr<ItemTrait> clone() const = 0;
    virtual ItemTraitType type() const = 0;
    virtual const char *traitName() const = 0;

    static std::unique_ptr<ItemTrait> createItemTraitByName(const std::string &name);

    virtual Json::Value serialize() const = 0;
    virtual void deserialize(const Json::Value &val) = 0;

protected:
    void serializeBase(Json::Value &val) const;
};

namespace Json {

template<>
struct Serializer<std::unique_ptr<ItemTrait> > {
    static Json::Value serialize(const std::unique_ptr<ItemTrait> &trait) {
        Json::Value val = trait->serialize();
        val["traitname"] = trait->traitName();
        return val;
    }

    static void deserialize(const Value &val, std::unique_ptr<ItemTrait> &trait) {
        Json::Value traitName = val.get("traitname", Json::Value());
        if (!traitName.isString()) { throw SerializationException("Expecting \"traitname\" string field in deserialize<std::unique_ptr<ItemTrait>"); }
        std::string name = traitName.asString();
        boost::algorithm::to_lower(name);
        trait = ItemTrait::createItemTraitByName(name);
        if (!trait) throw SerializationException("Invalid traitname \"" + name + "\"");
        trait->deserialize(val);
    }
};

}

#endif // ITEMTRAIT_H
