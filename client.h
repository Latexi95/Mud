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
		virtual void disconnect() = 0;
		RHandle<Player> player() const;
		void setPlayer(RHandle<Player> player);
		void setMessageHandler(const boost::shared_ptr<MessageHandler> &msgHandler);
	private:
		RHandle<Character> mCharacter;
		boost::shared_ptr<MessageHandler> mMessageHandler;
		RHandle<Player> mPlayer;
};

#endif // CLIENT_H
