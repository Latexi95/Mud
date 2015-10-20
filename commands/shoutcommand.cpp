#include "shoutcommand.h"

ShoutCommand::ShoutCommand() :
    Command("shout", "shout <message>") {

}

ShoutCommand::~ShoutCommand()
{

}

CommandResult ShoutCommand::execute(const CommandContext &c)
{
    return CommandResult();
}


