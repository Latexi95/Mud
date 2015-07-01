#ifndef GRIDLEVEL_H
#define GRIDLEVEL_H
#include "level.h"
#include <array>
#include "position.h"
#include "traits/roomtrait.h"

class GridLevelRoom;
class GridLevelWall;
class GridLevel : public Level {
		friend class GridLevelRoom;
		friend class GridLevelWall;
	public:
		GridLevel(const std::string &name, int width, int height);
		virtual ~GridLevel();
		virtual Room room(int localX, int localY);


	private:
		struct WallData {
			std::string mLooks;
			std::vector<std::unique_ptr<Item> > mItems;
		};

		struct RoomData {
			RoomData() : mWalls{0, 0, 0, 0, 0, 0}, mSolid(true) {}
			std::array<WallData *, Wall::SideCount> mWalls;
			std::vector<std::unique_ptr<RoomTrait>> mTraits;
			std::unordered_map<std::string, std::string> mLooks;
			bool mSolid;
		};

		RoomData *&roomData(int x, int y);

		RoomData **mRooms;
		int mWidth;
		int mHeight;

		RoomData mDefaultRoom;
		std::array<WallData, Wall::SideCount> mDefaultWalls;
		std::unordered_multimap<Position, std::shared_ptr<Character> > mCharacters;
};

#endif // GRIDLEVEL_H
