#ifndef PLAYEREVENTHANDLER_H
#define PLAYEREVENTHANDLER_H
#include "charactereventhandler.h"
#include "events/eventvisitor.h"
#include <memory>
class Client;
class PlayerEventHandler : public CharacterEventHandler, public EventVisitor
{
public:
    PlayerEventHandler(const std::shared_ptr<Client> &client);
    virtual ~PlayerEventHandler();

    virtual void handleEvent(const std::shared_ptr<Character> &character, Event *e);
private:
    virtual void visit(MessageEvent *e);
    virtual void visit(JoinEvent *e);
    virtual void visit(DisconnectEvent *e);
    virtual void visit(MoveStartEvent *e);
    virtual void visit(MoveEndEvent *e);

    std::shared_ptr<Character> mCharacter;
    std::shared_ptr<Client> mClient;
};

#endif // PLAYEREVENTHANDLER_H
