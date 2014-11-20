#ifndef GRIDLEVEL_H
#define GRIDLEVEL_H
#include "level.h"
class GridLevel : public Level {
	public:
		GridLevel(const std::string &name, int width, int height);
		virtual ~GridLevel();
		virtual Room &room(int localX, int localY);


	private:
		struct RoomData {
				std::vector<RHandle<Item> > mItems;

		};

		RoomData *mRooms;
		int mWidth;
		int mHeight;
};

#endif // GRIDLEVEL_H
