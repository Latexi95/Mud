#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H
#include <vector>
#include <memory>
class Event;
class Character;
class CharacterEventHandler {
public:
    CharacterEventHandler();
    virtual void handleEvent(const std::shared_ptr<Character> &character, Event *e) = 0;
};

#endif // EVENTHANDLER_H
