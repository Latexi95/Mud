#include "client.h"

Client::Client(const boost::shared_ptr<MessageHandler> &msgHandler) :
	mMessageHandler(msgHandler),
	mPlayer(0)
{
}

Client::~Client() {

}

void Client::receiveMessage(const std::string &message) {
	mMessageHandler->handle(this, message);
}

RHandle<Player> Client::player() const {
	return mPlayer;
}

void Client::setPlayer(RHandle<Player> player) {
	mPlayer = player;
}

void Client::setMessageHandler(const boost::shared_ptr<MessageHandler> &msgHandler) {
	mMessageHandler = msgHandler;
}
