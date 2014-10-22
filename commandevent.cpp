#include "commandevent.h"

CommandEvent::CommandEvent(const std::string &cmd, Item *sender) :
	Event(sender),
	mCommand(cmd) {
}
