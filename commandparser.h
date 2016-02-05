#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H
#include "commands/command.h"
#include <memory>
#include <vector>

class CommandParser {
public:
    CommandParser();
    ~CommandParser();
    Command *parse(const std::string &cmd, std::vector<std::string> &params);

    void addCommand(const std::shared_ptr<Command> &command);

    const std::string &errorMessage() const { return mErrorMessage; }

private:
    typedef std::vector<std::shared_ptr<Command> > CmdVector;
    CmdVector::iterator closestCommand(const std::string &base);

    CmdVector mCommands;
    std::string mErrorMessage;
};

#endif // COMMANDPARSER_H
