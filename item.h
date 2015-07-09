#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <vector>
#include "jsonserializable.h"
#include "name.h"
#include "box.h"
#include <memory>
#include <unordered_map>

class ItemTrait;
class Item : public JsonSerializable {
public:
    Item();
    Item(const std::string &path);
    Item(const Name &name);
    ~Item();
    void initFromBase(const std::shared_ptr<Item> &b);
    const Name &name() const;
    void setName(const Name &n);

    double weight() const { return mWeight; }
    void setWeight(double t) { mWeight = t; }
    const Box<double> &size() const { return mSize; }
    void setSize(const Box<double> &b) { mSize = b; }

    Json::Value serialize() const;
    void deserialize(const Json::Value &val);

    bool hasTrait(const std::string &name);
    const ItemTrait &trait(const std::string &name);

    std::unique_ptr<Item> clone() const;

    std::string path() const { return mPath; }
protected:
    std::shared_ptr<Item> mBase;
    std::string mPath;
    Name mName;
    double mWeight;
    Box<double> mSize;
    std::unordered_map<std::string, std::unique_ptr<ItemTrait> > mTraits;
};

#endif // ITEM_H
