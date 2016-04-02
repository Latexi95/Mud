#include "ui.h"
#include "client.h"
#include "item.h"

UI::UI(Client *c) :
    mClient(c)
{
}

void UI::send(const std::string &str)
{
    mClient->sendMessage(str);
}

void UI::send(const MessageBuilder &msg)
{
    mClient->sendMessage(msg);
}

void UI::commandError(const std::string &msg)
{
    mClient->sendMessage(msg);
}

void UI::commandError(const MessageBuilder &msg)
{
    mClient->sendMessage(msg);
}

Client *UI::client()
{
    return mClient;
}

const std::shared_ptr<Character> UI::character() const
{
    return mClient->character();
}

void UI::printDescription(Item *item)
{
    mClient->sendMessage(MessageBuilder() << item << ": " << item->description());
}
