#include "commandparser.h"
#include "command.h"
#include <boost/algorithm/string.hpp>
#include <boost/locale.hpp>
#include "textgen/textutils.h"

CommandParser::CommandParser(const std::shared_ptr<GameEventLoop> &global, const std::shared_ptr<GameEventLoop> &local) :
	mGlobalEventLoop(global),
	mLocalEventLoop(local)
{

}

CommandParser::~CommandParser() {
}

void CommandParser::addCommand(std::unique_ptr<Command> &&command) {
	auto insertIt = closestCommand(command->base());
	mCommands.emplace(insertIt, std::move(command));
}

CommandParser::CmdVector::iterator CommandParser::closestCommand(const std::string &base) {
	if (mCommands.empty()) return mCommands.end();
	CmdVector::iterator i = mCommands.begin();
	if (std::lexicographical_compare(base.begin(), base.end(), (*i)->base().begin(), (*i)->base().end())) {
		return i;
	}

	if (mCommands.size() == 1) {
		return mCommands.end();
	}

	CmdVector::iterator rangeStart = mCommands.begin();
	CmdVector::iterator rangeEnd = mCommands.end();

	while (true) {
		auto diff = (rangeEnd - rangeStart);
		if (diff == 1) {
			if (std::lexicographical_compare((*rangeStart)->base().begin(), (*rangeStart)->base().end(), base.begin(), base.end())) {
				return rangeStart;
			}
			else {
				return rangeEnd;
			}
		}
		i = rangeStart + diff / 2;
		if (std::lexicographical_compare((*i)->base().begin(), (*i)->base().end(), base.begin(), base.end())) {
			rangeStart = i;
		}
		else {
			rangeEnd = i;
		}
	}

}

Command *CommandParser::parse(const std::string &cmd, std::vector<std::string> &params) {
	mErrorMessage.clear();
	std::string trimmedCmd = boost::algorithm::trim_copy(cmd);

	auto firstSpaceIndex = trimmedCmd.find(' ');
	std::string cmdName;
	if (firstSpaceIndex == std::string::npos) {
		cmdName = trimmedCmd;
	}
	else {
		cmdName = trimmedCmd.substr(0, firstSpaceIndex);
	}
	cmdName = boost::locale::fold_case(cmdName);


	auto cmdIt = closestCommand(cmdName);
	if (cmdIt == mCommands.end()) {
		mErrorMessage = "Can't find command \"" + cmdName + "\".";
		return nullptr;
	}
	if (!boost::algorithm::starts_with((*cmdIt)->base(), cmdName)) {
		mErrorMessage = "Can't find command \"" + cmd + "\". Did you mean \"" + (*cmdIt)->base() + "\"?";
		return nullptr;
	}

	if ((*cmdIt)->base() != cmd) {
		auto possibleCommandsBegin = cmdIt;
		auto possibleCommandsEnd = cmdIt + 1;
		if (cmdIt != mCommands.begin()) {
			while (boost::algorithm::starts_with((*(possibleCommandsBegin - 1))->base(), cmdName)) {
				--possibleCommandsBegin;
				if (possibleCommandsBegin == mCommands.begin()) break;

			}
		}
		if (cmdIt != mCommands.end() - 1) {
			while (boost::algorithm::starts_with((*possibleCommandsEnd)->base(), cmdName)) {
				++possibleCommandsEnd;
				if (possibleCommandsEnd == mCommands.end()) break;
			}
		}

		if (possibleCommandsEnd - possibleCommandsBegin > 1) {
			mErrorMessage = "Multiple possible commands: ";
			for (auto i = possibleCommandsBegin; i != possibleCommandsEnd; ++i) {
				if (i != possibleCommandsBegin) {
					mErrorMessage += ", ";
				}
				mErrorMessage += (*i)->base();
			}
			return nullptr;
		}
	}

	if (firstSpaceIndex != std::string::npos) {
		auto paramListBegin = trimmedCmd.begin() + firstSpaceIndex;
		auto paramListEnd = trimmedCmd.end();
		for (auto i = paramListBegin; i != paramListEnd; ++i) {
			if (*i == ' ') continue;

			if (*i == '"') {
				++i;
				auto paramStart = i;
				while (i != paramListEnd && *i != '"') {
					++i;
				}
				if (i == paramListEnd) {
					mErrorMessage = "Expecting \" to close the quotation.";
					return nullptr;
				}

				params.emplace_back(paramStart, i);
			}

			auto paramStart = i;
			while (i != paramListEnd && *i != ' ') {
				++i;
			}
			params.emplace_back(paramStart, i);
			if (i == paramListEnd) break;
		}
	}


	return (*cmdIt).get();
}
