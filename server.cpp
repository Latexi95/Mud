#include "server.h"

class TcpServer {
	public:
		TcpServer(boost::asio::io_service &ioService) {

		}
};

Server::Server(bool useIPv6) :
	mUseIPv6(useIPv6)
{
}

void Server::start(int threads, const std::string &addr) {
	boost::asio::ip::tcp::resolver resolver(mIOService);
	boost::asio::ip::tcp::endpoint endpoint;
	if(mUseIPv6)
	{
		boost::asio::ip::tcp::resolver::query queryv6(boost::asio::ip::tcp::v6(), addr);
		endpoint = *resolver.resolve(queryv6);
	}
	else
	{
		boost::asio::ip::tcp::resolver::query queryv4(boost::asio::ip::tcp::v4(), addr);
		endpoint = *resolver.resolve(queryv4);
	}

	for (int i = 0; i < threads; i++) {
		mWorkers.push_back(new boost::thread(boost::bind(&boost::asio::io_service::run, &mIOService)));
	}
}

void Server::runServer(TCPServer *tcp, void (*HandleFunction)(shared_ptr<ip::tcp::socket>, deadline_timer *)) {

}

void Server::handleMessage(boost::shared_ptr<boost::asio::ip::tcp::socket> socket, boost::asio::deadline_timer *timer) {

}
