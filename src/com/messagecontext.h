#ifndef MESSAGECONTEXT_H
#define MESSAGECONTEXT_H
#include "messagebuilder.h"

class Client;

class MessageContext
{
public:
    MessageContext(Client *c);

    void send(const std::string &str);
    void send(const MessageBuilder &msg);

    void commandError(const std::string &msg);
    void commandError(const MessageBuilder &msg);
    Client *client();
private:
    Client *mClient;
};

#endif // MESSAGECONTEXT_H
