#include "commandevent.h"
#include "eventvisitor.h"

CommandEvent::CommandEvent(const std::string &cmd) :
    Event(),
    mCommand(cmd) {
}

void CommandEvent::execute()
{

}

IMPL_ACCEPT_EVENT_VISITOR(CommandEvent)
