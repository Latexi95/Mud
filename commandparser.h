#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H
#include "command.h"
#include <memory>
#include <vector>

class CommandParser {
public:
    CommandParser();
    ~CommandParser();
    Command *parse(const std::string &cmd, std::vector<std::string> &params);

    void addCommand(std::unique_ptr<Command> &&command);

    const std::string &errorMessage() const { return mErrorMessage; }

private:
    typedef std::vector<std::unique_ptr<Command> > CmdVector;
    CmdVector::iterator closestCommand(const std::string &base);

    CmdVector mCommands;
    std::string mErrorMessage;
};

#endif // COMMANDPARSER_H
