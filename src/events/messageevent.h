#ifndef MESSAGEEVENT_H
#define MESSAGEEVENT_H
#include <string>
#include <memory>
#include "event.h"
class Character;

class MessageEvent : public Event {
public:
    MessageEvent(const std::shared_ptr<Character> &sender, const std::string &msg);
    ~MessageEvent();

    EventType type() const { return EventType::Message; }

    const std::string &message() const { return mMessage; }
    const std::shared_ptr<Character> sender() const { return mSender; }
    DEF_ACCEPT_EVENT_VISITOR()
    virtual void execute();
private:
    std::shared_ptr<Character> mSender;
    std::string mMessage;
};

#endif // MESSAGEEVENT_H
