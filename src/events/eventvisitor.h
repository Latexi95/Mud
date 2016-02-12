#ifndef EVENTVISITOR_H
#define EVENTVISITOR_H
#define DEF_EVENT_VISIT(EventType) \
    virtual void visit(EventType *e);

class CommandEvent;
class JoinEvent;
class DisconnectEvent;
class MessageEvent;
class MoveStartEvent;
class MoveEndEvent;

class EventVisitor
{
public:
    EventVisitor();
    virtual ~EventVisitor();
    DEF_EVENT_VISIT(CommandEvent)
    DEF_EVENT_VISIT(JoinEvent)
    DEF_EVENT_VISIT(DisconnectEvent)
    DEF_EVENT_VISIT(MessageEvent)
    DEF_EVENT_VISIT(MoveStartEvent)
    DEF_EVENT_VISIT(MoveEndEvent)
};

#undef DEF_EVENT_VISIT

#endif // EVENTVISITOR_H
