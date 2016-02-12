#include "commandservice.h"
#include "events/event.h"
#include "events/commandevent.h"
#include "commands/movecommands.h"
#include "commands/shoutcommand.h"
#include "commands/lookcommand.h"
#include <iostream>
#include <cassert>

CommandService *CMDS = nullptr;

CommandService::CommandService()
{
    assert(CMDS == 0);
    CMDS = this;
    mStandardCommands = {
        std::make_shared<WalkCommand>(),
        std::make_shared<WalkCommand>(Direction::North),
        std::make_shared<WalkCommand>(Direction::East),
        std::make_shared<WalkCommand>(Direction::South),
        std::make_shared<WalkCommand>(Direction::West),
        std::make_shared<WalkCommand>(Direction::Up),
        std::make_shared<WalkCommand>(Direction::Down),
        std::make_shared<LookCommand>()
    };

    for (auto &c : mStandardCommands) {
        mStandardCommandParser.addCommand(c);
    }

}

CommandService::~CommandService()
{
    assert(CMDS);
    CMDS = nullptr;
}

const std::vector<std::shared_ptr<Command> > CommandService::standardCommands() const
{
    return mStandardCommands;
}

const CommandParser &CommandService::standardCommandParser() const
{
    return mStandardCommandParser;
}
