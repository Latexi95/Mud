#ifndef CLIENT_H
#define CLIENT_H
#include "resource.h"
#include "character.h"
#include "messagehandler.h"
class Client {
	public:
		Client(const boost::shared_ptr<MessageHandler> &msgHandler);
		virtual ~Client();
		virtual void sendMessage(const std::string &message) = 0;
		virtual void receiveMessage(const std::string &message);
	private:
		RHandle<Character> mCharacter;
		boost::shared_ptr<MessageHandler> mMessageHandler;
		Player *mPlayer;
};

#endif // CLIENT_H
