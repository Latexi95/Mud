#ifndef ENUMS_H_
#define ENUMS_H_

enum class Direction {
    //DO NOT CHANGE ORDER OR VALUES
    North = 0,
    South,
    East,
    West,
    Up,
    Down,
    Count
};

const char *directionToString(Direction d);
char directionToChar(Direction d);

enum class ItemTraitType {
    Weapon,
    Container,
    Locked,
    Landmark
};

enum class MoveType {
    Walk,
    Run,
    Crawl
};


#endif // ENUMS_H_

