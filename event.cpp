#include "event.h"

Event::Event(Item *sender) :
	mSender(sender),
	mAccepted(false) {
}

Event::~Event() {
}

void Event::accept() {
	mAccepted = true;
}

void Event::reject() {
	mAccepted = false;
}
