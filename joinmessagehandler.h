#ifndef JOINMESSAGEHANDLER_H
#define JOINMESSAGEHANDLER_H
#include "messagehandler.h"
class JoinMessageHandler : public MessageHandler {
	public:
		JoinMessageHandler();
		~JoinMessageHandler();
		void handle(Client *client, const std::string &message) const;
};

#endif // JOINMESSAGEHANDLER_H
