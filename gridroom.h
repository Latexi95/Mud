#ifndef GRIDROOM_H
#define GRIDROOM_H
#include "room.h"

class GridRoom : public Room {
	public:
		class Cell {
		public:
			Cell();
			~Cell();
			std::vector<Item*> items() const { return mItems; }
			void setItems(const std::vector<Item*> items) { mItems = items; }
			bool isMovableLocation() const;
		private:
			bool mWall;
			std::vector<Item*> mItems;
			int mAcrobaticsDifficulty;
		};

		GridRoom(const std::string &name, int w, int h);
		virtual ~GridRoom();

		virtual bool isMovableLocation(int x, int y) const;
		virtual std::vector<Item *> itemsInLocation(int x, int y) const;
	private:
		bool locationInGrid(int x, int y) const;
		Cell &cell(int x, int y);
		const Cell &cell(int x, int y) const;

		int mWidth;
		int mHeight;
		Cell *mGrid;
};

#endif // GRIDROOM_H
