#include "client.h"

Client::Client(const std::shared_ptr<MessageHandler> &msgHandler) :
    mMessageHandler(msgHandler),
    mPlayer(0)
{
}

Client::~Client() {

}

void Client::receiveMessage(const std::string &message) {
    mMessageHandler->handle(this, message);
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
