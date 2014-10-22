#ifndef SERVER_H
#define SERVER_H
#include <boost/smart_ptr/make_unique.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/atomic/atomic.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/array.hpp>

class TcpConnection : public boost::enable_shared_from_this<TcpConnection> {
	public:
		typedef boost::shared_ptr<TcpConnection> pointer;

		static pointer create(boost::asio::io_service& io_service, void (*disconnectHandler)(pointer), void (*msgHandler)(pointer, const std::string &)) {
			return pointer(new TcpConnection(io_service, disconnectHandler, msgHandler));
		}

		boost::asio::ip::tcp::socket& socket() {
			return mSocket;
		}

		void addMessage(std::string *str);

		void writeMessage();

		void startReceive();
	private:
		TcpConnection(boost::asio::io_service& io_service, void (*disconnectHandler)(pointer), void (*msgHandler)(pointer, const std::string &)):
			mSocket(io_service),
			mSendQueue(64),
			mCurrentMsg(0),
			mInputString(),
			mInputBuffer(),
			mMessageHandler(msgHandler),
			mDisconnectHandler(disconnectHandler)
		{

		}


		void handleWrite(const boost::system::error_code& err, size_t bytesTransfered);
		void handleRead(const boost::system::error_code& err, size_t bytesTransfered);

		boost::asio::ip::tcp::socket mSocket;
		boost::lockfree::queue<std::string*> mSendQueue;
		boost::atomic_flag mCurrentlySending;
		std::string *mCurrentMsg;
		std::string mInputString;
		boost::array<char, 256> mInputBuffer;
		void (*mMessageHandler)(TcpConnection::pointer, const std::string &);
		void (*mDisconnectHandler)(TcpConnection::pointer);
};

class TcpServer {
	public:
		TcpServer(void (*newConHandler)(TcpConnection::pointer), void (*disconnectHandler)(TcpConnection::pointer), void (*newMessageHandler)(TcpConnection::pointer, const std::string &), bool useIPv6);
		void start(int threads, unsigned short port);
		void stop();
	private:
		void startAccept();
		void handleAccept(TcpConnection::pointer newCon, const boost::system::error_code &err);

		boost::asio::io_service mIOService;
		std::vector<boost::thread*> mWorkers;
		bool mUseIPv6;
		std::unique_ptr<boost::asio::ip::tcp::acceptor> mAcceptor;
		void (*mNewConnectionHandler)(TcpConnection::pointer);
		void (*mDisconnectHandler)(TcpConnection::pointer);
		void (*mMessageHandler)(TcpConnection::pointer, const std::string &);
};



#endif // SERVER_H
