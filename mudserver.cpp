#include "mudserver.h"
#include <boost/bind/placeholders.hpp>
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

MudServer::MudServer() {
	sMudServerInstance = this;
	mTcpServer.reset(new TcpServer(
						 &sHandleNewCon,
						 &sHandleDisconnect,
						 &sHandleMessage,
						 false
	));
}

void MudServer::start() {
	mTcpServer->start(2, 3333);
}

void MudServer::stop() {
	mTcpServer->stop();
}

void MudServer::handleNewConnection(TcpConnection::pointer newCon) {
	newCon->addMessage(new std::string("Welcome to my mud server\r\n"));
	mClients[newCon] = boost::make_shared<Client>();
}

void MudServer::handleMessage(TcpConnection::pointer sender, const std::string &msg) {
	std::cout << '[' << sender << "] : " << msg << std::endl;
}

void MudServer::handleDisconnect(TcpConnection::pointer con) {
	mClients.erase(con);
}
