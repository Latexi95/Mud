#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H
#include "commands/command.h"
#include <memory>
#include <vector>
class Client;
class MessageContext;
class CommandParser {
public:
    CommandParser();
    ~CommandParser();
    bool parse(const std::string &cmd, CommandContext &&context, const std::shared_ptr<Client> &client) const;

    void addCommand(const std::shared_ptr<Command> &command);
private:
    typedef std::vector<std::shared_ptr<Command> > CmdVector;
    CmdVector::iterator closestCommand(const std::string &base);
    CmdVector::const_iterator closestCommand(const std::string &base) const;

    CmdVector mCommands;
};

#endif // COMMANDPARSER_H
