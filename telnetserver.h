#ifndef SERVER_H
#define SERVER_H
#include <boost/smart_ptr/make_unique.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/atomic/atomic.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/array.hpp>
#include <queue>
#include "libtelnet.h"

class TelnetConnection : public boost::enable_shared_from_this<TelnetConnection> {
    friend void telnetEvent(telnet_t *telnet, telnet_event_t *event, void *user_data);
public:
    typedef boost::shared_ptr<TelnetConnection> pointer;

    static pointer create(boost::asio::io_service& io_service, void (*disconnectHandler)(pointer), void (*msgHandler)(pointer, const std::string &)) {
        return pointer(new TelnetConnection(io_service, disconnectHandler, msgHandler));
    }

    ~TelnetConnection();

    boost::asio::ip::tcp::socket& socket() {
        return mSocket;
    }

    void addMessage(std::string *msg);

    void writeMessage();

    void startReceive();
    void disconnect();
private:
    TelnetConnection(boost::asio::io_service& io_service, void (*disconnectHandler)(pointer), void (*msgHandler)(pointer, const std::string &));

    void handleWrite(const boost::system::error_code& err, size_t bytesTransfered);
    void handleRead(const boost::system::error_code& err, size_t bytesTransfered);

    boost::asio::ip::tcp::socket mSocket;
    boost::lockfree::queue<std::string*> mSendQueue;
    boost::atomic_flag mCurrentlySending;
    std::string mOutputBuffer;
    std::vector<std::string> mInputQueue;
    boost::array<char, 256> mInputBuffer;
    void (*mMessageHandler)(TelnetConnection::pointer, const std::string &);
    void (*mDisconnectHandler)(TelnetConnection::pointer);
    telnet_t *mTelnet;
    boost::mutex mTelnetMutex;
};

class TelnetServer {
public:
    TelnetServer(void (*newConHandler)(TelnetConnection::pointer), void (*disconnectHandler)(TelnetConnection::pointer), void (*newMessageHandler)(TelnetConnection::pointer, const std::string &), bool useIPv6);
    void start(int threads, unsigned short port);
    void stop();
private:
    void startAccept();
    void handleAccept(TelnetConnection::pointer newCon, const boost::system::error_code &err);

    boost::asio::io_service mIOService;
    std::vector<boost::thread*> mWorkers;
    bool mUseIPv6;
    std::unique_ptr<boost::asio::ip::tcp::acceptor> mAcceptor;
    void (*mNewConnectionHandler)(TelnetConnection::pointer);
    void (*mDisconnectHandler)(TelnetConnection::pointer);
    void (*mMessageHandler)(TelnetConnection::pointer, const std::string &);
};



#endif // SERVER_H
