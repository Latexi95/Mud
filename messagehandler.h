#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include <string>
class Client;
class MessageHandler {
	public:
		MessageHandler();
		virtual ~MessageHandler();
		virtual void handle(Client *client, const std::string &message) const = 0;
};

#endif // MESSAGEHANDLER_H
