#include "enums.h"
#include <cassert>

const char *directionToString(Direction d)
{
    static const char *sDirectionNames[] = {
        "North",
        "South",
        "East",
        "West",
        "Up",
        "Down"
    };

    assert((unsigned)d < (unsigned)Direction::Count);

    return sDirectionNames[(unsigned)d];
}

char directionToChar(Direction d)
{
    static const char sDirectionChars[] = {
        'n',
        's',
        'e',
        'w',
        'u',
        'd'
    };
    assert((unsigned)d < (unsigned)Direction::Count);

    return sDirectionChars[(unsigned)d];
}
