#include "messageevent.h"
#include "level.h"
#include "eventvisitor.h"

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

IMPL_ACCEPT_EVENT_VISITOR(MessageEvent)

