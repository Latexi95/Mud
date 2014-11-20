#ifndef JOINMESSAGEHANDLER_H
#define JOINMESSAGEHANDLER_H
#include "messagehandler.h"

class JoinMessageHandler : public MessageHandler {
	public:
		JoinMessageHandler();
		~JoinMessageHandler();
		void handle(Client *client, const std::string &message);

	private:
		enum State {
			ExpectingName,
			ExpectingPassword,
			ExpectingNewPassword,
			ExpectingPasswordConfirmation
		} mState;
};

#endif // JOINMESSAGEHANDLER_H
