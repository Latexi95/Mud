#include "telnetserver.h"
#include <boost/thread/lock_guard.hpp>
#include <boost/asio/basic_stream_socket.hpp>

static const telnet_telopt_t telopts[] = {
  { TELNET_TELOPT_ECHO,      TELNET_WONT, TELNET_DO },
  { TELNET_TELOPT_TTYPE,     TELNET_WILL, TELNET_DONT },
  { TELNET_TELOPT_COMPRESS2, TELNET_WILL, TELNET_DO   },
  { TELNET_TELOPT_ZMP,       TELNET_WONT, TELNET_DO   },
  { TELNET_TELOPT_MSSP,      TELNET_WONT, TELNET_DO   },
  { TELNET_TELOPT_BINARY,    TELNET_WILL, TELNET_DO   },
  { TELNET_TELOPT_NAWS,      TELNET_WILL, TELNET_DONT },
  { -1, 0, 0 }
};

TelnetServer::TelnetServer(void (*newConHandler)(TelnetConnection::pointer), void (*disconnectHandler)(TelnetConnection::pointer), void (*newMessageHandler)(TelnetConnection::pointer, const std::string &), bool useIPv6):
	mUseIPv6(useIPv6),
	mNewConnectionHandler(newConHandler),
	mDisconnectHandler(disconnectHandler),
	mMessageHandler(newMessageHandler)
{

}

void TelnetServer::start(int threads, unsigned short port) {
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

void TelnetServer::stop() {
	mIOService.stop();
}

void TelnetServer::startAccept(){
	TelnetConnection::pointer con = TelnetConnection::create(mIOService, mDisconnectHandler, mMessageHandler);
	mAcceptor->async_accept(con->socket(), boost::bind(
								&TelnetServer::handleAccept,
								this,
								con,
								boost::asio::placeholders::error));
}

void TelnetServer::handleAccept(TelnetConnection::pointer newCon, const boost::system::error_code &err) {
	if (!err) {
		mNewConnectionHandler(newCon);
		newCon->startReceive();
	}
	startAccept();
}


TelnetConnection::~TelnetConnection() {
	if (mTelnet)
		telnet_free(mTelnet);
}

void TelnetConnection::addMessage(std::string *msg) {
	mSendQueue.push(msg);
	if (!mCurrentlySending.test_and_set()) {
		writeMessage();
	}
}

void TelnetConnection::writeMessage() {
	std::string *msg;
	if (mSendQueue.pop(msg)) {
		{boost::lock_guard<boost::mutex> guard(mTelnetMutex);
			telnet_send(mTelnet, msg->c_str(), msg->size());
		}
		delete msg;
	}
	else {
		mCurrentlySending.clear();
	}
}

void TelnetConnection::startReceive() {
	mSocket.async_read_some(boost::asio::buffer(mInputBuffer),
							boost::bind(
								&TelnetConnection::handleRead, shared_from_this(),
								boost::asio::placeholders::error,
								boost::asio::placeholders::bytes_transferred
								)
							);
}

void TelnetConnection::disconnect() {
	mSocket.close();
}

void telnetEvent(telnet_t *telnet, telnet_event_t *event, void *user_data) {
	TelnetConnection *con = static_cast<TelnetConnection*>(user_data);
	switch (event->type) {
		//Send data to client
		case TELNET_EV_SEND: {
			boost::asio::async_write(con->mSocket,
									 boost::asio::buffer(event->data.buffer, event->data.size),
									 boost::bind(
										 &TelnetConnection::handleWrite, con->shared_from_this(),
										 boost::asio::placeholders::error,
										 boost::asio::placeholders::bytes_transferred));
			break;
		}
		case TELNET_EV_DATA: {
			if (event->data.size > 0) {
				if (event->data.size == 2 && event->data.buffer[0] == '\r' && event->data.buffer[1] == '\n') {
					break;
				}
				con->mInputQueue.emplace_back(event->data.buffer, event->data.size);
			}
			break;
		}
	}
}

TelnetConnection::TelnetConnection(boost::asio::io_service& io_service, void (*disconnectHandler)(pointer), void (*msgHandler)(pointer, const std::string &)):
	mSocket(io_service),
	mSendQueue(64),
	mInputBuffer(),
	mMessageHandler(msgHandler),
	mDisconnectHandler(disconnectHandler),
	mTelnet(0)
{
	mTelnet = telnet_init(telopts, telnetEvent, 0, this);
}

void TelnetConnection::handleWrite(const boost::system::error_code &err, size_t bytesTransfered) {
	if (err) {
		std::cerr << err.message();
		if (err == boost::asio::error::connection_reset || err == boost::asio::error::eof) {
			mDisconnectHandler(shared_from_this());
			return;
		}
	}
	writeMessage();
}

void TelnetConnection::handleRead(const boost::system::error_code &err, size_t bytesTransfered) {
	if (!err) {
		std::vector<std::string> input;
		{boost::lock_guard<boost::mutex> guard(mTelnetMutex);
			telnet_recv(mTelnet, mInputBuffer.begin(), bytesTransfered);
			input = std::move(mInputQueue);
			mInputQueue.clear();
		}
		for (const std::string &str : input) {
			mMessageHandler(shared_from_this(), str);
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

