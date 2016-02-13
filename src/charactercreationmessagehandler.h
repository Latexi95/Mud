#ifndef CHARACTERCREATIONMESSAGEHANDLER_H
#define CHARACTERCREATIONMESSAGEHANDLER_H
#include "messagehandler.h"
#include "character.h"
#include <queue>
#include "util/color.h"
class CharacterCreationMessageHandler : public MessageHandler {
public:
    CharacterCreationMessageHandler();
    ~CharacterCreationMessageHandler();

    void sendCharacterCreationInitMessage(const std::shared_ptr<Client> &client);
    void handle(const std::shared_ptr<Client> &client, const std::string &message);
    void disconnected(const std::shared_ptr<Client> &client);
private:
    enum Status {
        InitMessage,
        GenderSelection,
        NameSelection,
        AgeSelection,
        HairColorSelection,
        InsertToMap
    } mStatus;

    std::queue<Status> mStatusStack;

    std::string mName;
    Character::Gender mGender;
    int mAge;
    text::Color *mHairColor;

};

#endif // CHARACTERCREATIONMESSAGEHANDLER_H
