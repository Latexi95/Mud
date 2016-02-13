#include "messagecontext.h"
#include "client.h"

MessageContext::MessageContext(Client *c) :
    mClient(c)
{
}

void MessageContext::send(const std::string &str)
{
    mClient->sendMessage(str);
}

void MessageContext::send(const MessageBuilder &msg)
{
    mClient->sendMessage(msg);
}

void MessageContext::commandError(const std::string &msg)
{
    mClient->sendMessage(msg);
}

void MessageContext::commandError(const MessageBuilder &msg)
{
    mClient->sendMessage(msg);
}

Client *MessageContext::client()
{
    return mClient;
}
