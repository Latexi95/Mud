#ifndef UI_H
#define UI_H
#include "messagebuilder.h"

class Client;
class Character;
class Item;

class UI
{
public:
    UI(Client *c);

    void send(const std::string &str);
    void send(const MessageBuilder &msg);

    void commandError(const std::string &msg);
    void commandError(const MessageBuilder &msg);
    Client *client();

    const std::shared_ptr<Character> character() const;

    void printDescription(Item *item);
private:
    Client *mClient;
};

#endif // UI_H
