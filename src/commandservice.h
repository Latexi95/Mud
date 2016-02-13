#ifndef COMMANDSERVICE_H
#define COMMANDSERVICE_H
#include "charactereventhandler.h"
#include "commandparser.h"
class Command;
class CommandEvent;

class CommandService {
public:
    CommandService();
    ~CommandService();

    const std::vector<std::shared_ptr<Command>> standardCommands() const;
    const CommandParser &standardCommandParser() const;


    const std::vector<std::shared_ptr<Command> > &editorCommands() const;
    const CommandParser &editorCommandParser() const;

private:
    std::vector<std::shared_ptr<Command>> mStandardCommands;
    std::vector<std::shared_ptr<Command>> mEditorCommands;
    CommandParser mStandardCommandParser;
    CommandParser mEditorCommandParser;
};

extern CommandService *CMDS;

#endif // COMMANDSERVICE_H
