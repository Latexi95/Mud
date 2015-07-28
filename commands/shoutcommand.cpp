#include "shoutcommand.h"

ShoutCommand::ShoutCommand() :
    Command("shout", "shout <message>", 1, 1) {

}

ShoutCommand::~ShoutCommand()
{

}

void ShoutCommand::execute(const CommandContext &c)
{

}

