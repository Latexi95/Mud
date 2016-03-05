#include "util/enums.h"
#include <boost/algorithm/string.hpp>
#include <cassert>
#include "textselector.h"
#include "util/textutils.h"

static const char *sDirectionNames[] = {
    "north",
    "south",
    "east",
    "west",
    "up",
    "down"
};


const char *DirectionToString(Direction d)
{

    assert((unsigned)d < (unsigned)Direction::Count);

    return sDirectionNames[(unsigned)d];
}

static const char sDirectionChars[] = {
    'n',
    's',
    'e',
    'w',
    'u',
    'd'
};

char DirectionToChar(Direction d)
{

    assert((unsigned)d < (unsigned)Direction::Count);

    return sDirectionChars[(unsigned)d];
}

Direction DirectionFromString(std::string txt)
{
    boost::algorithm::trim(txt);
    boost::algorithm::to_lower(txt);
    if (txt.length() == 1) {
        for (unsigned i = 0; i < (unsigned)Direction::Count; ++i) {
            if (txt[0] == sDirectionChars[i]) {
                return (Direction)i;
            }
        }
    }
    else {
        for (unsigned i = 0; i < (unsigned)Direction::Count; ++i) {
            if (txt == sDirectionNames[i]) {
                return (Direction)i;
            }
        }
    }
    return Direction::Invalid;
}

static TextSelectorMap<ItemTraitType> ItemTraitTypeSelector = {
    {"weapon", ItemTraitType::Weapon},
    {"container", ItemTraitType::Container},
    {"locked", ItemTraitType::Locked},
    {"landmark", ItemTraitType::Landmark},
    {"eatable", ItemTraitType::Eatable}};

ItemTraitType ItemTraitTypeFromString(std::string txt)
{
    text::selectClean(txt);
    try {
        return ItemTraitTypeSelector.match(txt);
    }
    catch (TextSelectorError){
        return ItemTraitType::Invalid;
    }
}

static const char *itemTraitTypes[] = {
    "weapon",
    "container",
    "locked",
    "landmark",
    "eatable",
    "invalid"
};

const char *ItemTraitTypeToString(ItemTraitType type)
{
    unsigned i = (int) type;
    assert(i <= (unsigned)ItemTraitType::Invalid);
    return itemTraitTypes[i];
}
