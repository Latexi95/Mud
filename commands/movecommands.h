#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H
#include "command.h"
#include "enums.h"

class WalkCommand : public Command
{
public:
    WalkCommand();
    WalkCommand(Direction direction);

    bool globalEvent() const { return false; }

    bool execute(const CommandContext &c, MessageContext &messageContext) const;

    bool isShortcut() const { return mShortcut != Direction::Count; }

protected:
    Direction mShortcut;
};

#endif // MOVECOMMAND_H
