#ifndef COLOR_H
#define COLOR_H
#include <string>
#include <vector>
namespace text {
	class Color {
		public:
			Color() : mName(), mDescription(), mAltNames(), mRGB(0) {}
			static void loadColors();
			static void saveColors();
		private:
			std::string mName;
			std::string mDescription;
			std::vector<std::string> mAltNames;
			int mRGB;
	};

	Color *colorByName(const std::string &name);


}

#endif // COLOR_H
