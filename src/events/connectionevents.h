#ifndef JOINEVENT_H
#define JOINEVENT_H
#include "event.h"
#include <memory>

class Character;
class Client;
class JoinEvent : public Event
{
public:
    JoinEvent(const std::shared_ptr<Client> &client, const std::shared_ptr<Character> &character);
    EventType type() const { return EventType::Join; }
    void execute();

    const std::shared_ptr<Client> &client() const { return mClient; }
    const std::shared_ptr<Character> &character() const { return mCharacter; }
    DEF_ACCEPT_EVENT_VISITOR()
private:
    std::shared_ptr<Client> mClient;
    std::shared_ptr<Character> mCharacter;
};


class DisconnectEvent : public Event
{
public:
    DisconnectEvent(const std::shared_ptr<Client> &client, const std::shared_ptr<Character> &character);
    EventType type() const { return EventType::Disconnect; }
    void execute();

    const std::shared_ptr<Client> &client() const { return mClient; }
    const std::shared_ptr<Character> &character() const { return mCharacter; }
    DEF_ACCEPT_EVENT_VISITOR()
private:
    std::shared_ptr<Client> mClient;
    std::shared_ptr<Character> mCharacter;
};
#endif // JOINEVENT_H
