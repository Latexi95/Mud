#include "mudserver.h"
#include <boost/bind/placeholders.hpp>
#include "joinmessagehandler.h"
#include "client.h"
static MudServer *sMudServerInstance = 0;
void sHandleNewCon(TcpConnection::pointer ptr) {
	sMudServerInstance->handleNewConnection(ptr);
}

void sHandleDisconnect(TcpConnection::pointer ptr) {
	sMudServerInstance->handleDisconnect(ptr);
}

void sHandleMessage(TcpConnection::pointer ptr, const std::string &msg) {
	sMudServerInstance->handleMessage(ptr, msg);
}


void MudClient::sendMessage(const std::string &message) {
	std::string *output = new std::string();
	output->resize(message.size() + 2);
	std::copy(message.begin(), message.end(), output->begin());
	output[message.size()] = '\r';
	output[message.size() + 1] = '\n';
	mConnection->addMessage(output);
}


MudServer::MudServer() {
	sMudServerInstance = this;
	mTcpServer.reset(new TcpServer(
						 &sHandleNewCon,
						 &sHandleDisconnect,
						 &sHandleMessage,
						 false
	));
	mJoinHandler = boost::shared_ptr<MessageHandler>(new JoinMessageHandler());
}

void MudServer::start() {
	mTcpServer->start(2, 3333);
}

void MudServer::stop() {
	mTcpServer->stop();
}

void MudServer::handleNewConnection(TcpConnection::pointer newCon) {
	newCon->addMessage(new std::string("Welcome to my mud server\r\n"));
	newCon->addMessage(new std::string("What's our name adventurer?\r\n"));
	mClients[newCon] = boost::make_shared<MudClient>(mJoinHandler, newCon);
}

void MudServer::handleMessage(TcpConnection::pointer sender, const std::string &msg) {
	std::cout << '[' << sender << "] : " << msg << std::endl;
	mClients[sender]->receiveMessage(msg);
}

void MudServer::handleDisconnect(TcpConnection::pointer con) {
	mClients.erase(con);
}

