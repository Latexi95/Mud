#include "commandparser.h"
#include "command.h"
#include <boost/algorithm/string.hpp>
#include <boost/locale.hpp>
#include "textgen/textutils.h"

CommandParser::CommandParser()
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


    {
        CmdVector::iterator i = mCommands.begin();
        if (std::lexicographical_compare(base.begin(), base.end(), (*i)->base().begin(), (*i)->base().end())) {
            return i;
        }
    }

    if (mCommands.size() == 1) {
        return mCommands.end();
    }

    CmdVector::iterator rangeStart = mCommands.begin();
    CmdVector::iterator rangeEnd = mCommands.end();


    auto strLessEqual = [](const std::string &a, const std::string &b) {
        return !std::lexicographical_compare(b.begin(), b.end(), a.begin(), a.end());
    };

    int i = 0;
    int n = (rangeEnd - rangeStart);
    for (int b = n / 2; b >= 1; b /= 2) {
        while (i + b < n && strLessEqual(rangeStart[i + b]->base(), base)) {
            i += b;
        }
    }

    return rangeStart + i;
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
