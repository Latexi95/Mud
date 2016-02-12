#ifndef EVENT_H
#define EVENT_H
#include <utility>

class Item;
class EventVisitor;

#define DEF_ACCEPT_EVENT_VISITOR() \
    virtual void accept(EventVisitor *v);
#define IMPL_ACCEPT_EVENT_VISITOR(CLASS) \
    void CLASS::accept(EventVisitor *v) { v->visit(this); }

enum class EventType {
    Command,
    Function,
    Message,
    MoveStart,
    MoveEnd,
    Join,
    Disconnect
};

class Event {
public:

    Event();
    virtual ~Event();
    virtual EventType type() const = 0;

    void takeOwnership() { mEventLoopHasOwnership = false; }

    bool eventLoopHasOwnership() const { return mEventLoopHasOwnership; }
    virtual void execute() = 0;
    virtual void accept(EventVisitor *v) = 0;
    virtual bool isGlobalEvent() { return false; }
protected:
    bool mEventLoopHasOwnership;
};

template <typename Func>
class FunctionEvent : public Event {
public:
    FunctionEvent(const Func &func) : mFunc(func) {}
    FunctionEvent(Func &&func) : mFunc(func) {}
    virtual ~FunctionEvent() {}

    virtual EventType type() const { return EventType::Function; }
    virtual void accept(EventVisitor *) {}
    virtual void execute() {
        mFunc(this);
    }
private:
    Func mFunc;
};
template <typename Func>
FunctionEvent<Func> *makeFunctionEvent(Func &&func) {
    return new FunctionEvent<Func>(std::move(func));
}

template <typename Func>
FunctionEvent<Func> *makeFunctionEvent(const Func &func) {
    return new FunctionEvent<Func>(func);
}

#endif // EVENT_H
