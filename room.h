#ifndef ROOM_H
#define ROOM_H
#include <vector>
#include <string>
class Item;
class Room {
	public:
		Room(const std::string &name);
		virtual ~Room();

		std::string name() const;
		void setName(const std::string &value);
		std::vector<Item*> items() const { return mItems; }

		virtual bool isMovableLocation(int x, int y) const = 0;
		virtual std::vector<Item*> itemsInLocation(int x, int y) const = 0;
	protected:
		std::string mName;
		std::vector<Item*> mItems;

};

#endif // ROOM_H
