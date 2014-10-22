#include "tcpserver.h"


TcpServer::TcpServer(void (*newConHandler)(TcpConnection::pointer), void (*disconnectHandler)(TcpConnection::pointer), void (*newMessageHandler)(TcpConnection::pointer, const std::string &), bool useIPv6):
	mUseIPv6(useIPv6),
	mNewConnectionHandler(newConHandler),
	mDisconnectHandler(disconnectHandler),
	mMessageHandler(newMessageHandler)
{

}

void TcpServer::start(int threads, unsigned short port) {
	boost::asio::ip::tcp::endpoint endpoint;
	if(mUseIPv6) {
		endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v6(), port);
	}
	else {
		endpoint = boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port);
	}

	mAcceptor.reset(new boost::asio::ip::tcp::acceptor(mIOService, endpoint));
	startAccept();

	for (int i = 0; i < threads; i++) {
		mWorkers.push_back(new boost::thread([&]() {
			std::cout << "Started thread " << boost::this_thread::get_id() << std::endl;
			mIOService.run();
			std::cout << "Stopped thread " << boost::this_thread::get_id() << std::endl;
		}));
	}

}

void TcpServer::stop() {
	mIOService.stop();
}

void TcpServer::startAccept(){
	TcpConnection::pointer con = TcpConnection::create(mIOService, mDisconnectHandler, mMessageHandler);
	mAcceptor->async_accept(con->socket(), boost::bind(
								&TcpServer::handleAccept,
								this,
								con,
								boost::asio::placeholders::error));
}

void TcpServer::handleAccept(TcpConnection::pointer newCon, const boost::system::error_code &err) {
	if (!err) {
		mNewConnectionHandler(newCon);
		newCon->startReceive();
	}
	startAccept();
}


void TcpConnection::addMessage(std::string *str) {
	mSendQueue.push(str);
	if (!mCurrentlySending.test_and_set()) {
		writeMessage();
	}
}

void TcpConnection::writeMessage() {
	if (mSendQueue.pop(mCurrentMsg)) {
		boost::asio::async_write(mSocket, boost::asio::buffer(*mCurrentMsg), boost::bind(
									 &TcpConnection::handleWrite, shared_from_this(),
									 boost::asio::placeholders::error,
									 boost::asio::placeholders::bytes_transferred));
	}
	else {
		mCurrentlySending.clear();
	}
}

void TcpConnection::startReceive() {
	boost::asio::async_read(mSocket, boost::asio::buffer(mInputBuffer),
							[&] (const boost::system::error_code &err, std::size_t bytesTransferred) -> std::size_t {
								if (bytesTransferred > 0 && mInputBuffer.at(bytesTransferred - 1) == '\n') return 0;
								return bytesTransferred - mInputBuffer.size();
							},
							boost::bind(
								&TcpConnection::handleRead, shared_from_this(),
								boost::asio::placeholders::error,
								boost::asio::placeholders::bytes_transferred
								)
							);
}

void TcpConnection::handleWrite(const boost::system::error_code &err, size_t bytesTransfered) {
	if (err) {
		std::cerr << err.message();
		if (err == boost::asio::error::connection_reset || err == boost::asio::error::eof) {
			mDisconnectHandler(shared_from_this());
			return;
		}
	}
	delete mCurrentMsg;
	writeMessage();
}

void TcpConnection::handleRead(const boost::system::error_code &err, size_t bytesTransfered) {
	if (!err) {
		int start = mInputString.size();
		mInputString.resize(mInputString.size() + bytesTransfered);
		std::copy(mInputBuffer.begin(), mInputBuffer.begin() + bytesTransfered, mInputString.begin() + start);
		std::string::const_iterator i = mInputString.begin() + start;
		std::string::const_iterator lastPartEnd = mInputString.begin();
		if (*i == '\n' || *i == '\0') {
			lastPartEnd = i + 1;
			std::string emptyString;
			mMessageHandler(shared_from_this(), emptyString);
		}
		++i;
		while (true) {
			if (i == mInputString.end()) {
				break;
			}
			if (*i == '\n' || *i == '\0') {
				i++;
				if (i == mInputString.end()) {
					if (lastPartEnd == mInputString.begin()) {
						mMessageHandler(shared_from_this(), mInputString);
					}
					else {
						std::string msg(lastPartEnd, i - 2);
						mMessageHandler(shared_from_this(), msg);
					}
					lastPartEnd = mInputString.end();
					break;
				}
				else {
					std::string msg(lastPartEnd, i - 2);
					mMessageHandler(shared_from_this(), msg);
					lastPartEnd = i;
				}
			}
			else {
				i++;
			}
		}
		if (lastPartEnd != mInputString.end()) {
			if (lastPartEnd != mInputString.begin()) {
				mInputString = std::string(lastPartEnd, mInputString.cend());
			}
		}
		else {
			mInputString.clear();
		}
	}
	else {
		std::cerr << err.message();
		if (err == boost::asio::error::connection_reset || err == boost::asio::error::eof) {
			mDisconnectHandler(shared_from_this());
			return;
		}
	}

	startReceive();
}

