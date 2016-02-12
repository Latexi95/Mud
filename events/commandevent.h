#ifndef COMMANDEVENT_H
#define COMMANDEVENT_H
#include "event.h"
#include <string>
#include "commands/command.h"
class Client;
class Command;
class CommandEvent : public Event {
public:
    CommandEvent(Command *cmd, CommandContext &&ctx, const std::shared_ptr<Client> &client);
    EventType type() const { return EventType::Command; }
    virtual void execute();
    DEF_ACCEPT_EVENT_VISITOR()
private:
    Command *mCommand;
    CommandContext mContext;
    std::shared_ptr<Client> mClient;
};

#endif // COMMANDEVENT_H
