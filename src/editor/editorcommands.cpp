#include "editorcommands.h"
#include "characterservice.h"
#include "item.h"
#include "messagecontext.h"
#include "client.h"
#include "editor.h"
#include "util/textutils.h"
#include "editorservice.h"
#include <boost/algorithm/string.hpp>

using namespace editor;

StartEditingCommand::StartEditingCommand() :
    Command("edit", "edit [item item_id]/[room room_id]/[level level_id]", 2, 2)
{

}

enum class EditType {
    Item,
    Room,
    Level
};
bool StartEditingCommand::execute(const CommandContext &c, MessageContext &messageContext) const
{
    std::shared_ptr<Character> character = c.mCaller;

    std::string editType = c.mParameters[0];
    boost::to_lower(editType);

    std::string id = text::lowered(c.mParameters[1]);
    text::clean(id);

    EditType eTy;
    if (editType == "item") {
        eTy = EditType::Item;
    }
    else if (editType == "room") {
        messageContext.commandError(MessageBuilder() << "Not implemented");
        eTy = EditType::Room;
        return false;
    }
    else if (editType == "level") {
        messageContext.commandError(MessageBuilder() << "Not implemented");
        eTy = EditType::Level;
        return false;
    }
    else {
        messageContext.commandError(MessageBuilder() << "Expecting \"item\", \"room\" or \"level\" as the first parameter");
        return false;
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
        if (ES->startEditingItem(client, id)) {
            return true;
        }
        else {
            return false;
        }
    }


    return false;
}

QuitEditingCommand::QuitEditingCommand() :
    Command("quit", "quit", 0, 0)
{

}

bool QuitEditingCommand::execute(const CommandContext &c, MessageContext &messageContext) const
{
    Client *client = messageContext.client();
    auto currentEditor = client->editor();
    if (currentEditor) {
        currentEditor->tryClose();
        return true;
    }
    else {
        messageContext.commandError("  Not currently editing");
        return false;
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

bool AnswerCommand::execute(const CommandContext &c, MessageContext &messageContext) const
{
    auto e = messageContext.client()->editor();
    if (e) {
        e->answer(mAnswer);
    }
}



SetCommand::SetCommand() :
    Command("set", "set id value", 2)
{
}

bool SetCommand::execute(const CommandContext &c, MessageContext &messageContext) const
{
    auto e = messageContext.client()->editor();
    if (!e) {
        messageContext.commandError(MessageBuilder() << "  Not currently editing anything");
        return false;
    }

    std::string id = text::lowered(c.mParameters[0]);
    text::clean(id);
    std::string value = c.mParameters[1];
    e->handleSet(id, value);
}

GetCommand::GetCommand() :
    Command("get", "get id", 1)
{
}

bool GetCommand::execute(const CommandContext &c, MessageContext &messageContext) const
{
    auto e = messageContext.client()->editor();
    if (!e) {
        messageContext.commandError(MessageBuilder() << "  Not currently editing anything");
        return false;
    }

    std::string id = text::lowered(c.mParameters[0]);
    text::clean(id);
    e->handleGet(id);
}
