#ifndef ENUMS_H_
#define ENUMS_H_
#include <string>

enum class Direction {
    //DO NOT CHANGE ORDER OR VALUES
    North = 0,
    South,
    East,
    West,
    Up,
    Down,
    Count,
    Invalid = Count
};

const char *DirectionToString(Direction d);
char DirectionToChar(Direction d);
Direction DirectionFromString(std::string txt);

template <typename Func>
void DirectionForEach(Func &&f) {
    for (unsigned i = 0; i < (unsigned)Direction::Count; ++i) {
        f((Direction)i);
    }
}

enum class ItemTraitType {
    Weapon,
    Container,
    Locked,
    Landmark,
    Eatable
};

enum class MoveType {
    Walk,
    Run,
    Crawl
};


enum class ContainerState {
    Open,
    Closed,
    Opened
};


#endif // ENUMS_H_

