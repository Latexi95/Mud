#include "movecommands.h"
#include <cassert>
#include "textgen/textselector.h"
#include "character.h"
#include "level.h"
#include "events/moveevent.h"
#include "characterservice.h"
#include "messagecontext.h"



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

bool WalkCommand::execute(const CommandContext &c, MessageContext &messageContext) const
{
    Direction dir;
    if (isShortcut()) {
        dir = mShortcut;
    }
    else {
        dir = DirectionFromString(c.mParameters.front());
    }

    if (dir == Direction::Invalid) {
        messageContext.commandError("Invalid direction");
        return false;
    }

    if (CS->canMove(c.mCaller, dir)) {
        CS->startMove(c.mCaller, dir);
    }
    else {
        messageContext.commandError(MessageBuilder("Can't move to ") << DirectionToString(dir));
        return false;
    }

    return true;
}
