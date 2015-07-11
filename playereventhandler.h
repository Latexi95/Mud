#ifndef PLAYEREVENTHANDLER_H
#define PLAYEREVENTHANDLER_H
#include "eventhandler.h"
#include <memory>
class Client;
class PlayerEventHandler : public EventHandler
{
public:
    PlayerEventHandler(const std::shared_ptr<Client> &client);
    ~PlayerEventHandler();

    void handleEvent(Event *e);

private:
    std::shared_ptr<Client> mClient;
};

#endif // PLAYEREVENTHANDLER_H
