#ifndef PLAYEREVENTHANDLER_H
#define PLAYEREVENTHANDLER_H
#include "charactereventhandler.h"
#include <memory>
class Client;
class PlayerEventHandler : public CharacterEventHandler
{
public:
    PlayerEventHandler(const std::shared_ptr<Client> &client);
    ~PlayerEventHandler();

    void handleEvent(const std::shared_ptr<Character> &character, Event *e);

private:
    std::shared_ptr<Client> mClient;
};

#endif // PLAYEREVENTHANDLER_H
