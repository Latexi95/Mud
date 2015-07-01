#ifndef CHARACTERCREATIONMESSAGEHANDLER_H
#define CHARACTERCREATIONMESSAGEHANDLER_H
#include "messagehandler.h"
#include "character.h"
#include <queue>
#include "textgen/color.h"
class CharacterCreationMessageHandler : public MessageHandler {
	public:
		CharacterCreationMessageHandler();
		~CharacterCreationMessageHandler();

		void sendCharacterCreationInitMessage(Client *c);
		void handle(Client *c, const std::string &message);
	private:
		enum Status {
			InitMessage,
			GenderSelection,
			NameSelection,
			AgeSelection,
			HairColorSelection,
			BackgroundSelection
		} mStatus;

		std::queue<Status> mStatusStack;

		std::string mName;
		Character::Gender mGender;
		int mAge;
		text::Color *mHairColor;

};

#endif // CHARACTERCREATIONMESSAGEHANDLER_H
