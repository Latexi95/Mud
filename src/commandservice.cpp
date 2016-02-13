#include "commandservice.h"
#include "events/event.h"
#include "events/commandevent.h"
#include "commands/movecommands.h"
#include "commands/shoutcommand.h"
#include "commands/lookcommand.h"
#include "editor/editorcommands.h"
#include <iostream>
#include <cassert>

CommandService *CMDS = nullptr;

using namespace editor;
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

    mEditorCommands = {
        std::make_shared<SetCommand>(),
        std::make_shared<GetCommand>(),
        std::make_shared<StartEditingCommand>(),
        std::make_shared<QuitEditingCommand>(),
        std::make_shared<AnswerCommand>(Answer::Yes),
        std::make_shared<AnswerCommand>(Answer::No),
        std::make_shared<AnswerCommand>(Answer::Cancel)
    };

    for (auto &c : mStandardCommands) {
        mStandardCommandParser.addCommand(c);
    }

    for (auto &c : mEditorCommands) {
        mEditorCommandParser.addCommand(c);
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

const std::vector<std::shared_ptr<Command> > &CommandService::editorCommands() const
{
    return mEditorCommands;
}

const CommandParser &CommandService::editorCommandParser() const
{
    return mEditorCommandParser;
}
