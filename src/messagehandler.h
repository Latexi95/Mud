#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include <string>
#include <memory>

class Client;
class MessageHandler {
public:
    MessageHandler();
    virtual ~MessageHandler();
    virtual void handle(const std::shared_ptr<Client> &client, const std::string &message) = 0;
    virtual void disconnected(const std::shared_ptr<Client> &client) = 0;
};

#endif // MESSAGEHANDLER_H
