#include "eventvisitor.h"


EventVisitor::EventVisitor()
{

}

EventVisitor::~EventVisitor()
{

}

#define DEF_EVENT_VISIT(EventType) \
    void EventVisitor::visit(EventType *) { }

DEF_EVENT_VISIT(CommandEvent)
DEF_EVENT_VISIT(JoinEvent)
DEF_EVENT_VISIT(DisconnectEvent)
DEF_EVENT_VISIT(MessageEvent)
DEF_EVENT_VISIT(MoveStartEvent)
DEF_EVENT_VISIT(MoveEndEvent)
