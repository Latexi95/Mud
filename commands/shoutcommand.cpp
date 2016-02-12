#include "shoutcommand.h"

ShoutCommand::ShoutCommand() :
    Command("shout", "shout <message>") {

}

ShoutCommand::~ShoutCommand()
{

}

bool ShoutCommand::execute(const CommandContext &c, MessageContext &messageContext) const
{
    return false;
}


