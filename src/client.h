#ifndef CLIENT_H
#define CLIENT_H
#include <memory>

#include "character.h"
#include "com/messagehandler.h"
#include "com/messagebuilder.h"
#include "com/ui.h"

class Player;
namespace editor {
class BaseEditor;
}
class Client : public std::enable_shared_from_this<Client> {
public:
    Client(const std::shared_ptr<MessageHandler> &msgHandler);
    virtual ~Client();
    virtual void sendMessage(const std::string &message) = 0;
    virtual void sendMessage(const MessageBuilder &mb) = 0;
    virtual void receiveMessage(const std::string &message);
    virtual void disconnect() = 0;
    virtual void disconnected();
    std::shared_ptr<Player> player() const;
    void setPlayer(const std::shared_ptr<Player> &player);
    void setMessageHandler(const std::shared_ptr<MessageHandler> &msgHandler);
    UI &ui();

    const std::shared_ptr<editor::BaseEditor> &editor() const;
    void setEditor(const std::shared_ptr<editor::BaseEditor> &e);
private:
    std::shared_ptr<Character> mCharacter;
    std::shared_ptr<MessageHandler> mMessageHandler;
    std::shared_ptr<Player> mPlayer;
    std::shared_ptr<editor::BaseEditor> mEditor;
    UI mMessageContext;
};

#endif // CLIENT_H
