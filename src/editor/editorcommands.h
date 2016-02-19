#ifndef EDITORCOMMANDS_H
#define EDITORCOMMANDS_H
#include "../commands/command.h"
enum class Answer {
    Yes = 0,
    No,
    Cancel
};

namespace editor {

class StartEditingCommand : public Command
{
public:
    StartEditingCommand();
    bool globalEvent() const { return true; }
    bool execute(const CommandContext &c, MessageContext &messageContext) const;
private:
};

class QuitEditingCommand : public Command
{
public:
    QuitEditingCommand();
    bool globalEvent() const { return true; }
    bool execute(const CommandContext &c, MessageContext &messageContext) const;
};

class AnswerCommand : public Command {
public:
    AnswerCommand(Answer a);
    bool execute(const CommandContext &c, MessageContext &messageContext) const;
private:
    Answer mAnswer;
};

class SetCommand : public Command
{
public:
    SetCommand();
    bool execute(const CommandContext &c, MessageContext &messageContext) const;
};

class GetCommand : public Command
{
public:
    GetCommand();
    bool execute(const CommandContext &c, MessageContext &messageContext) const;
};


enum class ListCommandParameter {
    Properties,
    Traits
};
class ListCommand : public Command
{
public:
    ListCommand();
    bool execute(const CommandContext &c, MessageContext &messageContext) const;
};

}

#endif // EDITORCOMMANDS_H
