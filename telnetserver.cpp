#include "telnetserver.h"
#include <boost/thread/lock_guard.hpp>
#include <boost/asio/basic_stream_socket.hpp>

static const telnet_telopt_t telopts[] = {
    { TELNET_TELOPT_ECHO,      TELNET_WONT, TELNET_DO   },
    { TELNET_TELOPT_TTYPE,     TELNET_WILL, TELNET_DONT },
    #ifdef HAVE_ZLIB
    { TELNET_TELOPT_COMPRESS2, TELNET_WILL, TELNET_DO   },
    #endif
    { TELNET_TELOPT_ZMP,       TELNET_WONT, TELNET_DO   },
    { TELNET_TELOPT_MSSP,      TELNET_WONT, TELNET_DO   },
    { TELNET_TELOPT_BINARY,    TELNET_WILL, TELNET_DO   },
    { TELNET_TELOPT_NAWS,      TELNET_WILL, TELNET_DONT },
    { TELNET_TELOPT_LINEMODE,  TELNET_WILL, TELNET_DO   },
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

    for (int i = 0; i < threads; i++) {
        mWorkers.push_back(new boost::thread([&]() {
            std::cout << "Started thread " << boost::this_thread::get_id() << std::endl;
            mIOService.run();
            std::cout << "Stopped thread " << boost::this_thread::get_id() << std::endl;
        }));
    }

    startAccept();

}

void TelnetServer::stop() {
    mIOService.stop();
    for (boost::thread *worker : mWorkers) {
        if (worker->joinable()) worker->join();
    }
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
        newCon->setupLineMode();

        mNewConnectionHandler(newCon);
        newCon->startReceive();
    }
    else {
        std::cerr << "handleAccept: " << err.message() << std::endl;
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

void TelnetConnection::setupLineMode()
{
    telnet_subnegotiation(mTelnet, TELNET_TELOPT_LINEMODE, "EDIT", 4);
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
            size_t start = 0;
            size_t i = 1;
            for (; i < event->data.size; ++i) {
                if (event->data.buffer[i - 1] == '\r' && event->data.buffer[i] == '\n') {
                    const char *startPtr = event->data.buffer + start;
                    size_t size = i - start - 1;
                    if (con->mIncompleteMessage) {
                        std::string &m = con->mInputQueue.back();
                        size_t oldSize = m.size();
                        m.resize(oldSize + size);
                        std::copy(startPtr, startPtr + size, &m[oldSize]);
                        start = i + 1;
                        con->mIncompleteMessage = false;
                    }
                    else {
                        con->mInputQueue.emplace_back(startPtr, size);
                        start = i + 1;
                    }
                }
            }
            if (start != event->data.size) {
                size_t size = event->data.size - start;
                const char *startPtr = event->data.buffer + start;
                if (con->mIncompleteMessage) {
                    std::string &m = con->mInputQueue.back();
                    size_t oldSize = m.size();
                    m.resize(oldSize + size);
                    std::copy(startPtr, startPtr + size, &m[oldSize]);
                    start = i + 1;
                }
                else {
                    con->mInputQueue.emplace_back(startPtr, size);
                }
                con->mIncompleteMessage = true;
            }
        }
        break;
    }
    default:
        break;
    }
}

TelnetConnection::TelnetConnection(boost::asio::io_service& io_service, void (*disconnectHandler)(pointer), void (*msgHandler)(pointer, const std::string &)):
    mSocket(io_service),
    mSendQueue(64),
    mIncompleteMessage(false),
    mInputBuffer(),
    mMessageHandler(msgHandler),
    mDisconnectHandler(disconnectHandler),
    mTelnet(0)
{
    mTelnet = telnet_init(telopts, telnetEvent, 0, this);
}

void TelnetConnection::handleWrite(const boost::system::error_code &err, size_t bytesTransfered) {
    if (err) {
        std::cerr << "handleWrite: " << err.message() << std::endl;
        /*if (err == boost::asio::error::connection_reset || err == boost::asio::error::eof) {

        }*/
        mDisconnectHandler(shared_from_this());
        return;
    }
    writeMessage();
}

void TelnetConnection::handleRead(const boost::system::error_code &err, size_t bytesTransfered) {
    if (!err) {
        std::vector<std::string> input;
        std::cout << "read: " << std::string(mInputBuffer.begin(), bytesTransfered) << std::endl;
        {boost::lock_guard<boost::mutex> guard(mTelnetMutex);
            telnet_recv(mTelnet, mInputBuffer.begin(), bytesTransfered);
            if (mIncompleteMessage && mInputQueue.size() == 1) {
                return;
            }
            input = std::move(mInputQueue);
            mInputQueue.clear();
            if (mIncompleteMessage) {
                mInputQueue.emplace_back(std::move(input.back()));
                input.erase(input.end() - 1);
            }
        }
        for (const std::string &str : input) {
            mMessageHandler(shared_from_this(), str);
        }
    }
    else {
        std::cerr << "handleRead: " << err.message() << std::endl;
        /*if (err == boost::asio::error::connection_reset || err == boost::asio::error::eof) {

        }*/
        mDisconnectHandler(shared_from_this());
        return;
    }

    startReceive();
}

