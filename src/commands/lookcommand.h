#ifndef LOOKCOMMAND_H
#define LOOKCOMMAND_H
#include "command.h"

class LookCommand : public Command
{
public:
    LookCommand();
    void execute(const CommandContext &c, UI &ui) const;
private:

};

#endif // LOOKCOMMAND_H
