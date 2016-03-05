#ifndef UI_H
#define UI_H
#include "messagebuilder.h"

class Client;

class UI
{
public:
    UI(Client *c);

    void send(const std::string &str);
    void send(const MessageBuilder &msg);

    void commandError(const std::string &msg);
    void commandError(const MessageBuilder &msg);
    Client *client();
private:
    Client *mClient;
};

#endif // UI_H
