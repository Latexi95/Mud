#ifndef MUDSERVER_H
#define MUDSERVER_H
#include "tcpserver.h"
#include <unordered_map>
#include <boost/make_shared.hpp>
class Client {

};
class Mud;
class MudServer {
	public:
		MudServer();
		void start();
		void stop();

		void handleNewConnection(TcpConnection::pointer newCon);
		void handleMessage(TcpConnection::pointer sender, const std::string &msg);
		void handleDisconnect(TcpConnection::pointer con);
	private:
		Mud *mMud;
		std::unique_ptr<TcpServer> mTcpServer;
		std::map<TcpConnection::pointer, boost::shared_ptr<Client> > mClients;
};

#endif // MUDSERVER_H
