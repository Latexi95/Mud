#ifndef EVENT_H
#define EVENT_H
#include <utility>

class Item;
class Event {
	public:
		enum Type {
			Command,
			Function,
			Message
		};

		Event();
		virtual ~Event();

		virtual Type type() const = 0;
		void takeOwnership() { mEventLoopHasOwnership = false; }

		bool eventLoopHasOwnership() const { return mEventLoopHasOwnership; }
		virtual void execute() = 0;


	protected:
		bool mEventLoopHasOwnership;
};

template <typename Func>
class FunctionEvent : public Event {
	public:
		FunctionEvent(const Func &func) : mFunc(func) {}
		FunctionEvent(Func &&func) : mFunc(func) {}
		~FunctionEvent() {}

		Type type() const { return Function; }

		void execute() {
			mFunc(this);
		}
	private:
		Func mFunc;
};
template <typename Func>
FunctionEvent<Func> *makeFunctionEvent(Func &&func) {
	return new FunctionEvent<Func>(std::move(func));
}

template <typename Func>
FunctionEvent<Func> *makeFunctionEvent(const Func &func) {
	return new FunctionEvent<Func>(func);
}

#endif // EVENT_H
