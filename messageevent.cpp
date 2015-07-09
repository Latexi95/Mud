#include "messageevent.h"
#include "level.h"

MessageEvent::MessageEvent(const std::shared_ptr<Character> &sender, const std::string &msg) :
    mSender(sender),
    mMessage(msg)
{

}

MessageEvent::~MessageEvent()
{

}

void MessageEvent::execute() {
    mSender->level()->sendEventToCharacters(this);
}
