#ifndef EVENT_H
#define EVENT_H
class Item;
class Event {
	public:
		enum Type {
			Command
		};

		Event(Item *sender);
		virtual ~Event();
		Item *sender() const { return mSender; }

		virtual Type type() const = 0;
		void accept();
		void reject();
		bool accepted() const { return mAccepted; }

	protected:
		Item *mSender;
		bool mAccepted;
};

#endif // EVENT_H
