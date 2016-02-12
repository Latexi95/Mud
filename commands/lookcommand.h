#ifndef LOOKCOMMAND_H
#define LOOKCOMMAND_H
#include "command.h"

class LookCommand : public Command
{
public:
    LookCommand();
    bool execute(const CommandContext &c, MessageContext &messageContext) const;
private:

};

#endif // LOOKCOMMAND_H
