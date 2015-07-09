#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H
#include <vector>
class Event;
class EventHandler {
public:
    EventHandler();
    virtual void handleEvent(Event *e) = 0;
};

#endif // EVENTHANDLER_H
