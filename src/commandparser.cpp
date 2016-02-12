#include "commandparser.h"
#include "commands/command.h"
#include <boost/algorithm/string.hpp>
#include <boost/locale.hpp>
#include "textgen/textutils.h"
#include "messagecontext.h"
#include "events/commandevent.h"
#include "maineventqueue.h"
#include "character.h"
#include "level.h"
#include "client.h"

CommandParser::CommandParser()
{

}

CommandParser::~CommandParser() {
}

void CommandParser::addCommand(const std::shared_ptr<Command> &command) {
    auto insertIt = closestCommand(command->base());
    mCommands.emplace(insertIt, std::move(command));
}

bool strLess(const std::string &a, const std::string &b) {
    return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
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

    int i = 0;
    int n = (rangeEnd - rangeStart);
    for (int b = n / 2; b >= 1; b /= 2) {
        while (i + b < n && strLess(rangeStart[i + b]->base(), base)) {
            i += b;
        }
    }

    return rangeStart + i + 1;
}

CommandParser::CmdVector::const_iterator CommandParser::closestCommand(const std::string &base) const
{
    if (mCommands.empty()) return mCommands.end();

    {
        CmdVector::const_iterator i = mCommands.begin();
        if (std::lexicographical_compare(base.begin(), base.end(), (*i)->base().begin(), (*i)->base().end())) {
            return i;
        }
    }

    if (mCommands.size() == 1) {
        return mCommands.end();
    }

    CmdVector::const_iterator rangeStart = mCommands.begin();
    CmdVector::const_iterator rangeEnd = mCommands.end();

    int i = 0;
    int n = (rangeEnd - rangeStart);
    for (int b = n / 2; b >= 1; b /= 2) {
        while (i + b < n && strLess(rangeStart[i + b]->base(), base)) {
            i += b;
        }
    }

    return rangeStart + i + 1;
}

bool CommandParser::parse(const std::string &cmd, CommandContext &&context, const std::shared_ptr<Client> &client) const {
    std::string trimmedCmd = cmd;
    boost::trim(trimmedCmd);

    MessageContext &messageContext = client->msgCtx();

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
        messageContext.send("Can't find command \"" + cmdName + "\".");
        return false;
    }
    if ((*cmdIt)->base() != cmdName) {
        auto possibleCommandsBegin = cmdIt;
        auto possibleCommandsEnd = cmdIt + 1;
        if (cmdIt != mCommands.begin()) {
            while (boost::algorithm::starts_with((*(possibleCommandsBegin - 1))->base(), cmdName)) {
                --possibleCommandsBegin;
                if (possibleCommandsBegin == mCommands.begin()) break;

            }
        }
        if (boost::algorithm::starts_with((*cmdIt)->base(), cmdName)) {
            if (cmdIt != mCommands.end() - 1) {
                while (boost::algorithm::starts_with((*possibleCommandsEnd)->base(), cmdName)) {
                    ++possibleCommandsEnd;
                    if (possibleCommandsEnd == mCommands.end()) break;
                }
            }
        }
        else {
            --possibleCommandsEnd;
        }

        size_t possiblities = possibleCommandsEnd - possibleCommandsBegin;
        if (possiblities > 1) {
            std::string errorMessage = "Multiple possible commands: ";
            for (auto i = possibleCommandsBegin; i != possibleCommandsEnd; ++i) {
                if (i != possibleCommandsBegin) {
                    errorMessage += ", ";
                }
                errorMessage += (*i)->base();
            }
            messageContext.send(errorMessage);
            return false;
        }
        else if (possiblities == 0) {
            messageContext.send("Can't find command \"" + cmdName + "\".");
            return false;
        }
        else {
            cmdIt = possibleCommandsBegin;
        }
    }


    std::vector<std::string> params;
    if (firstSpaceIndex != std::string::npos) {
        if ((*cmdIt)->singleParameter()) {
             params.emplace_back(trimmedCmd.substr(firstSpaceIndex + 1));
        }
        else {
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
                        messageContext.send("Expecting \" to close the quotation.");
                        return false;
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
    }

    {
        Command *cmd = (*cmdIt).get();

        if (cmd->singleParameter()) {
            if (params.empty()) {
                messageContext.send("Expecting a parameter.\r\n" + cmd->usage());
                return false;
            }
        }

        unsigned minParams = cmd->minParameters();
        unsigned maxParams = cmd->maxParameters();
        if (params.size() < minParams) {
            messageContext.send("Too few parameters.\r\n" + cmd->usage());
            return false;
        } else if (params.size() > maxParams) {
            messageContext.send("Too many parameters.\r\n" + cmd->usage());
            return false;
        }

        context.mParameters = std::move(params);
        std::shared_ptr<Character> caller = context.mCaller;
        CommandEvent *event = new CommandEvent(cmd, std::move(context), client);
        if (event->isGlobalEvent()) {
            MEQ->push(event);
        }
        else {
            caller->level()->eventQueue()->push(event);
        }
        return true;
    }
}
