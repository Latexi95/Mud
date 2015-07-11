#ifndef JOINMESSAGEHANDLER_H
#define JOINMESSAGEHANDLER_H
#include "messagehandler.h"

class JoinMessageHandler : public MessageHandler {
public:
    JoinMessageHandler();
    ~JoinMessageHandler();
    void handle(const std::shared_ptr<Client> &client, const std::string &message);
    void disconnected(const std::shared_ptr<Client> &client);

    void gotoCharacterCreation(const std::shared_ptr<Client> &client);
private:
    enum State {
        ExpectingName,
        ExpectingPassword,
        ExpectingNewPassword,
        ExpectingPasswordConfirmation,
        CharacterSelection
    } mState;
};

#endif // JOINMESSAGEHANDLER_H
