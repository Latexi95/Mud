#include "commandevent.h"

CommandEvent::CommandEvent(const std::string &cmd) :
    Event(),
    mCommand(cmd) {
}
