#ifndef POSITION_H
#define POSITION_H
#include <functional>
class Position {
	public:
		Position();
		Position(int x, int y);
		int x() const;
		int y() const;
		bool operator == (const Position &p) const;
		Position &operator=(const Position &p) = default;

		int mX;
		int mY;
};

namespace std {
template <>
struct hash<Position> {
	typedef Position argument_type;
	typedef std::size_t result_type;
	result_type operator()(const Position &p) const {
		return p.x() ^ (p.y() << 16);
	}
};
}


#endif // POSITION_H
