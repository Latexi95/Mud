#include "client.h"

Client::Client(const std::shared_ptr<MessageHandler> &msgHandler) :
    mMessageHandler(msgHandler),
    mPlayer(0),
    mMessageContext(this)
{
}

Client::~Client() {

}

void Client::receiveMessage(const std::string &message) {
    mMessageHandler->handle(shared_from_this(), message);
}

void Client::disconnected()
{
    mMessageHandler->disconnected(shared_from_this());
}

std::shared_ptr<Player> Client::player() const {
    return mPlayer;
}

void Client::setPlayer(const std::shared_ptr<Player> &player) {
    mPlayer = player;
}

void Client::setMessageHandler(const std::shared_ptr<MessageHandler> &msgHandler) {
    mMessageHandler = msgHandler;
}

MessageContext &Client::msgCtx()
{
    return mMessageContext;
}
