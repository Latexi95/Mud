#ifndef MOVEEVENT_H
#define MOVEEVENT_H
#include "event.h"
#include <memory>

class Character;
class Room;

class MoveStartEvent : public Event
{
public:
    MoveStartEvent(const std::shared_ptr<Character> &c, Room *targetRoom);
    virtual ~MoveStartEvent();

    virtual EventType type() const { return EventType::MoveStart; }

    virtual void execute();

    std::shared_ptr<Character> character() const;
    DEF_ACCEPT_EVENT_VISITOR()

protected:
    std::shared_ptr<Character> mCharacter;
    Room *mTargetRoom;
};

class MoveEndEvent : public Event
{
public:
    MoveEndEvent(const std::shared_ptr<Character> &c, Room *targetRoom);
    virtual ~MoveEndEvent();

    virtual EventType type() const { return EventType::MoveEnd; }

    virtual void execute();

    std::shared_ptr<Character> character() const;
    Room *targetRoom() const;
    
    DEF_ACCEPT_EVENT_VISITOR()
protected:
    std::shared_ptr<Character> mCharacter;
    Room *mTargetRoom;
};

#endif // MOVESTARTEVENT_H
