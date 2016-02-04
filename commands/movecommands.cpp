#include "movecommands.h"
#include <cassert>
#include "textgen/textselector.h"
#include "character.h"
#include "level.h"
#include "events/moveevent.h"
#include "characterservice.h"



WalkCommand::WalkCommand() :
    Command("walk", "walk [north/south/east/west/up/down/towards xxx]"),
    mShortcut(Direction::Count)
{

}

WalkCommand::WalkCommand(Direction direction) :
    Command(),
    mShortcut(direction)
{
    switch (direction) {
    case Direction::North:
        mBase = "north";
        mUsage = "north";
        break;
    case Direction::South:
        mBase = "south";
        mUsage = "south";
        break;
    case Direction::West:
        mBase = "west";
        mUsage = "west";
        break;
    case Direction::East:
        mBase = "east";
        mUsage = "east";
        break;
    case Direction::Up:
        mBase = "up";
        mUsage = "up";
        break;
    case Direction::Down:
        mBase = "down";
        mUsage = "down";
        break;
    default:
        assert(0 && "Invalid direction");
    }
}

CommandResult WalkCommand::execute(const CommandContext &c)
{
    Direction dir;
    if (isShortcut()) {
        dir = mShortcut;
    }
    else {
        const std::string &direction = c.mParameters.front();
        std::string lower = boost::to_lower_copy(direction);

        TextSelectorMap<Direction> selector;
        selector.insert("north", Direction::North);
        selector.insert("south", Direction::South);
        selector.insert("east", Direction::East);
        selector.insert("west", Direction::West);
        selector.insert("up", Direction::Up);
        selector.insert("down", Direction::Down);

        try {
            dir = selector.value(lower);
        } catch (TextSelectorError err) {
            assert(err == TextSelectorError::NoMatches);

            if (boost::starts_with(lower, "toward ") || boost::starts_with(lower, "to ") || boost::starts_with(lower, "towards ")) {
                //TODO:
                CommandResult result;
                result.mSuccess = false;
                result.mErrorMessage = "\"walk to/towards\" is implemented yet";
                return result;
            }
            else {
                CommandResult result;
                result.mSuccess = false;
                result.mErrorMessage = usage();
                return result;
            }
        }
    }

    if (CS->canMove(c.mCaller, dir)) {
        CS->startMove(c.mCaller, dir);
    }
    else {
        CommandResult result;
        result.mSuccess = false;
        result.mErrorMessage = "Can't move to that direction";
    }


    CommandResult result;
    result.mSuccess = true;
    return result;
}
