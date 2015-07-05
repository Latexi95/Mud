#ifndef MESSAGEEVENT_H
#define MESSAGEEVENT_H
#include <string>
#include <memory>
#include "event.h"
class Character;

class MessageEvent : public Event {
	public:
		MessageEvent(const std::shared_ptr<Character> &sender, const std::string &msg);
		~MessageEvent();

		Type type() const { return Message; }

		const std::string &message() const { return mMessage; }
		const std::shared_ptr<Character> sender() const { return mSender; }

		void execute();
	private:
		std::shared_ptr<Character> mSender;
		std::string mMessage;
};

#endif // MESSAGEEVENT_H
