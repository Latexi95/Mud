#include "editorcommands.h"
#include "characterservice.h"
#include "item.h"
#include "com/ui.h"
#include "client.h"
#include "editor.h"
#include "util/textutils.h"
#include "editorservice.h"
#include <boost/algorithm/string.hpp>
#include "util/textselector.h"

using namespace editor;

StartEditingCommand::StartEditingCommand() :
    Command("edit", "edit [item <item_id>]/room <room_id>/[level <level_id>]", 2, 2)
{

}

enum class EditType {
    Item,
    Room,
    Level
};

static TextSelectorMap<EditType> itemRoomLevelSelector = {
    {"item", EditType::Item},
    {"room", EditType::Room},
    {"level", EditType::Level}};

void StartEditingCommand::execute(const CommandContext &c, UI &messageContext) const
{
    std::string editType = c.mParameters[0];
    text::selectClean(editType);

    std::string id = c.mParameters[1];
    text::selectClean(id);



    EditType eTy;
    try {
        eTy = itemRoomLevelSelector.match(editType);
    }
    catch (TextSelectorError e) {
        messageContext.commandError(MessageBuilder() << "Expecting \"item\", \"room\" or \"level\" as the first parameter");
        return;
    }

    Client *client = messageContext.client();
    auto currentEditor = client->editor();
    bool queueEditor = false;
    if (currentEditor) {
        TryCloseResult result = currentEditor->tryClose();
        switch (result) {
        case TryCloseResult::CantClose:
            //return;
        case TryCloseResult::Closed:
            break;
        case TryCloseResult::DelayedClose:
            queueEditor = true;
            break;
        }
    }

    switch (eTy) {
    case EditType::Item:
        ES->startEditingItem(client, id);
        break;
    }
}

QuitEditingCommand::QuitEditingCommand() :
    Command("quit", "quit", 0, 0)
{

}

void QuitEditingCommand::execute(const CommandContext &c, UI &messageContext) const
{
    Client *client = messageContext.client();
    auto currentEditor = client->editor();
    if (currentEditor) {
        currentEditor->tryClose();
    }
    else {
        messageContext.commandError("  Not currently editing");
    }
}

const char *answerString[] = {
    "yes",
    "no"
    "cancel"
};

AnswerCommand::AnswerCommand(Answer a) :
    Command(answerString[(int)a], answerString[(int)a], 0, 0),
    mAnswer(a)
{
}

void AnswerCommand::execute(const CommandContext &c, UI &messageContext) const
{
    auto e = messageContext.client()->editor();
    if (e) {
        e->answer(mAnswer);
    }
}



SetCommand::SetCommand() :
    Command("set", "set <id> <value>", 2)
{
}

void SetCommand::execute(const CommandContext &c, UI &messageContext) const
{
    auto e = messageContext.client()->editor();
    if (!e) {
        messageContext.commandError(MessageBuilder() << "  Not currently editing anything");
        return;
    }

    std::string id = c.mParameters[0];
    text::selectClean(id);
    std::string value = c.mParameters[1];
    e->handleSet(id, value);
}

GetCommand::GetCommand() :
    Command("get", "get <id>", 1)
{
}

void GetCommand::execute(const CommandContext &c, UI &messageContext) const
{
    auto e = messageContext.client()->editor();
    if (!e) {
        messageContext.commandError(MessageBuilder() << "  Not currently editing anything");
        return;
    }

    std::string id = text::lowered(c.mParameters[0]);
    text::clean(id);
    e->handleGet(id);
}


static TextSelectorMap<ListCommandParameter> listCommandOptionsSelector = {
    {"properties", ListCommandParameter::Properties},
    {"traits", ListCommandParameter::Traits}
};

ListCommand::ListCommand() :
    Command("list", "list properties/traits", 1, 1)
{
}

void ListCommand::execute(const CommandContext &c, UI &messageContext) const
{
    auto e = messageContext.client()->editor();
    if (!e) {
        messageContext.commandError(MessageBuilder() << "  Not currently editing anything");
        return;
    }

    std::string type = c.mParameters[0];
    text::selectClean(type);

    ListCommandParameter t;
    try {
        t = listCommandOptionsSelector.match(type);
    }
    catch (TextSelectorError e) {
        messageContext.commandError("  Expecting \"properties\" or \"traits\" after the command");
        return;
    }

    e->list(t);
}


static TextSelectorMap<AddType> addTypeSelector = {
    {"trait", AddType::Trait},
    {"item", AddType::Item},
    {"exit", AddType::Exit}
};

AddCommand::AddCommand() :
    Command("add", "add trait/item/exit <name/id>", 2)
{
}

void AddCommand::execute(const CommandContext &c, UI &messageContext) const
{
    auto e = messageContext.client()->editor();
    if (!e) {
        messageContext.commandError(MessageBuilder() << "  Not currently editing anything");
        return;
    }

    std::string type = c.mParameters.front();
    text::selectClean(type);

    std::string id = c.mParameters.back();
    text::clean(id);

    AddType addType;
    try {
        addType = addTypeSelector.match(type);
    }
    catch (TextSelectorError e) {
        messageContext.commandError("  Expecting \"trait\", \"item\" or \"exit\" after the command");
        return;
    }

    e->add(addType, id);
}

RemoveCommand::RemoveCommand() :
    Command("remove", "remove trait/item/exit <name/id>", 2)
{
}

void RemoveCommand::execute(const CommandContext &c, UI &messageContext) const
{
    auto e = messageContext.client()->editor();
    if (!e) {
        messageContext.commandError(MessageBuilder() << "  Not currently editing anything");
        return;
    }

    std::string type = c.mParameters.front();
    text::selectClean(type);

    std::string id = c.mParameters.back();
    text::clean(id);

    AddType addType;
    try {
        addType = addTypeSelector.match(type);
    }
    catch (TextSelectorError) {
        messageContext.commandError("  Expecting \"trait\", \"item\" or \"exit\" after the command");
        return;
    }

    e->remove(addType, id);
}
