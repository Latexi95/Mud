#include "shoutcommand.h"

ShoutCommand::ShoutCommand() :
    Command("shout", "shout <message>") {

}

ShoutCommand::~ShoutCommand()
{

}

void ShoutCommand::execute(const CommandContext &c, UI &messageContext) const
{
    return;
}


