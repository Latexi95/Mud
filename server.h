#ifndef SERVER_H
#define SERVER_H
#include <boost/smart_ptr/make_unique.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
class TcpServer;
class Server {
	public:
		Server(bool useIPv6);
		void start(int threads, const std::string &addr);
	private:
		boost::asio::io_service mIOService;
		std::vector<boost::thread*> mWorkers;
		bool mUseIPv6;
		TcpServer *mTcpServer;
};

#endif // SERVER_H
