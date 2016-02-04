#ifndef COMMANDEVENT_H
#define COMMANDEVENT_H
#include "event.h"
#include <string>

class CommandEvent : public Event {
public:
    CommandEvent(const std::string &cmd);
    Type type() const { return Command; }
    const std::string command() const { return mCommand; }
    virtual void execute();
    DEF_ACCEPT_EVENT_VISITOR()
private:
    std::string mCommand;
};

#endif // COMMANDEVENT_H
