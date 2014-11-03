#include "joinmessagehandler.h"
#include "client.h"
#include <boost/regex.h>
#include <boost/algorithm/string/trim_all.hpp>
JoinMessageHandler::JoinMessageHandler() {
}

JoinMessageHandler::~JoinMessageHandler() {

}

void JoinMessageHandler::handle(Client *client, const std::string &message) const {
	if (message.empty()) return;
	std::string trimmed = boost::algorithm::trim_all_copy(message);
	if (trimmed.empty()) {
		client->sendMessage("What's our name survivor?");
		return;
	}
	if (trimmed.size() < 2 || message.size() > 20) {
		client->sendMessage("Uh... I think that isn't our name."
							"(A character name should be between 2 and 20 characters.)");
		return;
	}
	client->sendMessage("Hello, " + message);
}
