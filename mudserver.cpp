#include "mudserver.h"
#include <boost/bind/placeholders.hpp>
#include "joinmessagehandler.h"
#include "client.h"
static MudServer *sMudServerInstance = 0;
void sHandleNewCon(TelnetConnection::pointer ptr) {
    sMudServerInstance->handleNewConnection(ptr);
}

void sHandleDisconnect(TelnetConnection::pointer ptr) {
    sMudServerInstance->handleDisconnect(ptr);
}

void sHandleMessage(TelnetConnection::pointer ptr, const std::string &msg) {
    sMudServerInstance->handleMessage(ptr, msg);
}


void MudClient::sendMessage(const std::string &message) {
    std::cout << '[' << mConnection << "] << " << message << std::endl;

    std::string *output = new std::string();
    output->resize(message.size() + 2);
    std::copy(message.begin(), message.end(), output->begin());
    (*output)[message.size()] = '\r';
    (*output)[message.size() + 1] = '\n';
    mConnection->addMessage(output);
}

void MudClient::sendMessage(const MessageBuilder &mb)
{
    std::string *output = new std::string(mb.generateTelnetString());

    std::cout << '[' << mConnection << "] << " << *output << std::endl;
    mConnection->addMessage(output);
}

void MudClient::disconnect() {
    mConnection->disconnect();
}


MudServer::MudServer() {
    sMudServerInstance = this;
    mTelnetServer.reset(new TelnetServer(
                         &sHandleNewCon,
                         &sHandleDisconnect,
                         &sHandleMessage,
                         false
                         ));
}

void MudServer::start() {
    mTelnetServer->start(2, 3334);
}

void MudServer::stop() {
    mTelnetServer->stop();
}

void MudServer::handleNewConnection(TelnetConnection::pointer newCon) {
    newCon->addMessage(new std::string("Welcome to my mud server\r\n"));
    newCon->addMessage(new std::string("Account name:\r\n"));
    mClients[newCon] = std::make_shared<MudClient>(std::shared_ptr<MessageHandler>(new JoinMessageHandler()), newCon);
}

void MudServer::handleMessage(TelnetConnection::pointer sender, const std::string &msg) {
    std::cout << '[' << sender << "] >> " << msg;
    mClients[sender]->receiveMessage(msg);
}

void MudServer::handleDisconnect(TelnetConnection::pointer con) {
    auto clientIt = mClients.find(con);
    if (clientIt != mClients.end()) {
        (clientIt->second)->disconnected();
        mClients.erase(clientIt);
    }
}

