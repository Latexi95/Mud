#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <vector>
#include "jsonserializable.h"
#include "resource.h"
#include "name.h"
class ItemTrait;
class Item : public JsonSerializable {
	public:
		Item();
		Item(const Name &name);
		virtual ~Item();
		void initFromBase(const RHandle<Item> &b);
		const Name &name() const;
		void setName(const Name &n);

		double weight() const { return mWeight; }
		void setWeight(double t) { mWeight = t; }
		double sizeX() const { return mSizeX; }
		void setSizeX(double w) { mSizeX = w; }
		double sizeY() const { return mSizeY; }
		void setSizeY(double h) { mSizeY = h; }
		double sizeZ() const { return mSizeZ; }
		void setSizeZ(double d) { mSizeZ = d; }
		const std::vector<ItemTrait*> &traits() const { return mTraits; }

		Json::Value serialize() const;
		bool deserialize(const Json::Value &val);
	protected:
		RHandle<Item> mBase;
		Name mName;
		double mWeight;
		double mSizeX;
		double mSizeY;
		double mSizeZ;
		std::unordered_map<std::string, ItemTrait*> mTraits;
};

#endif // ITEM_H
