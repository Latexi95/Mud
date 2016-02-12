#include "moveevent.h"
#include "character.h"
#include "room.h"
#include "eventvisitor.h"

MoveStartEvent::MoveStartEvent(const std::shared_ptr<Character> &c, Room *targetRoom) :
    mCharacter(c),
    mTargetRoom(targetRoom)
{
}

MoveStartEvent::~MoveStartEvent()
{

}

void MoveStartEvent::execute()
{
    Room *curRoom = mCharacter->room();
    curRoom->sendEventToCharacters(this);
}

std::shared_ptr<Character> MoveStartEvent::character() const
{
    return mCharacter;
}


MoveEndEvent::MoveEndEvent(const std::shared_ptr<Character> &c, Room *targetRoom) :
    mCharacter(c),
    mTargetRoom(targetRoom)
{
}

MoveEndEvent::~MoveEndEvent()
{

}

void MoveEndEvent::execute()
{
    Room *curRoom = mCharacter->room();
    curRoom->removeCharacter(mCharacter);
    mTargetRoom->addCharacter(mCharacter);
    mTargetRoom->sendEventToCharacters(this);
}

std::shared_ptr<Character> MoveEndEvent::character() const
{
    return mCharacter;
}

Room *MoveEndEvent::targetRoom() const
{
    return mTargetRoom;
}

IMPL_ACCEPT_EVENT_VISITOR(MoveEndEvent)
IMPL_ACCEPT_EVENT_VISITOR(MoveStartEvent)
