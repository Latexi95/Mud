#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <vector>
#include "jsonserializable.h"
#include "resource.h"
class Room;
class ItemTrait;
class Item : public JsonSerializable {
	public:
		Item();
		Item(const std::string &name);
		virtual ~Item();
		void initFromBase(const RHandle<Item> &b);
		const std::string &name() const;
		double weight() const { return mWeight; }
		void setWeight(double t) { mWeight = t; }
		double sizeX() const { return mSizeX; }
		void setSizeX(double w) { mSizeX = w; }
		double sizeY() const { return mSizeY; }
		void setSizeY(double h) { mSizeY = h; }
		double sizeZ() const { return mSizeZ; }
		void setSizeZ(double d) { mSizeZ = d; }
		Room *room() const { return mRoom; }
		void setRoom(Room *room) { mRoom = room; }
		const std::vector<ItemTrait*> &traits() const { return mTraits; }

		Json::Value serialize() const;
		bool deserialize(const Json::Value &val);
	protected:
		RHandle<Item> mBase;
		std::string mName;
		double mWeight;
		double mSizeX;
		double mSizeY;
		double mSizeZ;
		Room *mRoom;
		std::unordered_map<std::string, ItemTrait*> mTraits;
};

#endif // ITEM_H
