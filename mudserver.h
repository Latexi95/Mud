#ifndef MUDSERVER_H
#define MUDSERVER_H
#include "tcpserver.h"
#include <unordered_map>
#include <boost/make_shared.hpp>
#include "client.h"
class MudClient : public Client {
	public:
		MudClient(const boost::shared_ptr<MessageHandler> &msgHandler, TcpConnection::pointer con) :
			Client(msgHandler),
			mConnection(con) {}
		~MudClient() {}
		void sendMessage(const std::string &message);
	private:
		TcpConnection::pointer mConnection;
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
		std::map<TcpConnection::pointer, boost::shared_ptr<MudClient> > mClients;
		boost::shared_ptr<MessageHandler> mJoinHandler;
};

#endif // MUDSERVER_H
