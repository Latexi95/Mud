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
    void execute(const CommandContext &c, UI &messageContext) const;
private:
};

class QuitEditingCommand : public Command
{
public:
    QuitEditingCommand();
    bool globalEvent() const { return true; }
    void execute(const CommandContext &c, UI &messageContext) const;
};

class AnswerCommand : public Command {
public:
    AnswerCommand(Answer a);
    void execute(const CommandContext &c, UI &messageContext) const;
private:
    Answer mAnswer;
};

class SetCommand : public Command
{
public:
    SetCommand();
    void execute(const CommandContext &c, UI &messageContext) const;
};

class GetCommand : public Command
{
public:
    GetCommand();
    void execute(const CommandContext &c, UI &messageContext) const;
};


enum class ListCommandParameter {
    Properties,
    Traits
};
class ListCommand : public Command
{
public:
    ListCommand();
    void execute(const CommandContext &c, UI &messageContext) const;
};

enum class AddType {
    Trait,
    Item,
    Exit
};

class AddCommand : public Command
{
public:
    AddCommand();
    void execute(const CommandContext &c, UI &messageContext) const;
};

class RemoveCommand : public Command
{
public:
    RemoveCommand();
    void execute(const CommandContext &c, UI &messageContext) const;
};

}

#endif // EDITORCOMMANDS_H
