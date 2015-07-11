#ifndef MUDSERVER_H
#define MUDSERVER_H
#include "telnetserver.h"
#include <unordered_map>
#include <boost/make_shared.hpp>
#include "client.h"
class MudClient : public Client {
public:
    MudClient(const std::shared_ptr<MessageHandler> &msgHandler, TelnetConnection::pointer con) :
        Client(msgHandler),
        mConnection(con) {}
    ~MudClient() {}
    void sendMessage(const std::string &message);
    void sendMessage(const MessageBuilder &mb);
    void disconnect();
private:
    TelnetConnection::pointer mConnection;
};

class Mud;
class MudServer {
public:
    MudServer();
    void start();
    void stop();

    void handleNewConnection(TelnetConnection::pointer newCon);
    void handleMessage(TelnetConnection::pointer sender, const std::string &msg);
    void handleDisconnect(TelnetConnection::pointer con);
private:
    std::unique_ptr<TelnetServer> mTcpServer;
    std::map<TelnetConnection::pointer, std::shared_ptr<MudClient> > mClients;
};

#endif // MUDSERVER_H
