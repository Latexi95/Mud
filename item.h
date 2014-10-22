#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <vector>
#include "itemtrait.h"
#include "resource.h"
class Room;
class Item {
	public:
		Item();
		Item(const std::string &name);
		virtual ~Item();
		void initFromBase(const RHandle<Item> &b);
		const std::string &name() const;
		double weight() const { return mWeight; }
		void setWeight(double t) { mWeight = t; }
		double width() const { return mWidth; }
		void setWidth(double w) { mWidth = w; }
		double height() const { return mHeight; }
		void setHeight(double h) { mHeight = h; }
		double depth() const { return mDepth; }
		void setDepth(double d) { mDepth = d; }
		Room *room() const { return mRoom; }
		void setRoom(Room *room) { mRoom = room; }
		const std::vector<ItemTrait*> &traits() const { return mTraits; }
	protected:
		RHandle<Item> mBase;
		std::string mName;
		double mWeight;
		double mWidth;
		double mHeight;
		double mDepth;
		Room *mRoom;
		std::vector<ItemTrait*> mTraits;
};

#endif // ITEM_H
