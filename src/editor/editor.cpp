#include "editor.h"
#include "client.h"
#include "item.h"
#include "resourceservice.h"

using namespace editor;

static Properties<Item> sItemProperties;

BaseEditor::BaseEditor(Client *client) :
    mClient(client)
{
}

BaseEditor::~BaseEditor()
{

}

Client *BaseEditor::client() const
{
    return mClient;
}


std::shared_ptr<BaseEditor> BaseEditor::queuedEditor() const
{
    return mQueuedEditor;
}

void BaseEditor::setQueuedEditor(const std::shared_ptr<BaseEditor> &queuedEditor)
{
    mQueuedEditor = queuedEditor;
}


void BaseEditor::setupEditors()
{
    sItemProperties.addProperty("name", [](const Item *item) {
        return item->name().toParseString();
    },
    [](Item *item, const std::string &value) {
        item->setName(Name::fromParseString(value));
        return true;
    });
}

ItemEditor::ItemEditor(std::unique_ptr<Item> &&item, Client *client) :
    BaseEditor(client),
    mItem(std::move(item)),
    mState(State::Main)
{
    mContext.mTarget = mItem.get();
}

TryCloseResult ItemEditor::tryClose()
{
    mState = State::QuitQuestion;
    mClient->sendMessage("  Do you want to save item? (yes/no/cancel)");
    return TryCloseResult::DelayedClose;
}

void ItemEditor::answer(Answer a)
{
    switch (mState) {
    case State::Main:
        mClient->msgCtx().commandError("Ignored");
        break;
    case State::QuitQuestion:
        switch (a) {
        case Answer::Cancel:
            mQueuedEditor.reset();
            mState = State::Main;
            break;
        case Answer::Yes:
            saveAndQuit();
            break;
        case Answer::No:
            quit();
            break;
        }
    }
}

void ItemEditor::saveAndQuit()
{
    RS->storeItem(std::move(mItem));
    quit();
}

void ItemEditor::quit()
{
    mClient->setEditor(mQueuedEditor);
    mQueuedEditor.reset();
}


void ItemEditor::handleSet(const std::string &id, const std::string &value)
{
    MessageContext &msgCtx = mClient->msgCtx();
    if (!sItemProperties.hasProperty(id)) {
        msgCtx.commandError(MessageBuilder() << "  Can't find property " << id);
        return;
    }

    if (!sItemProperties.set(mContext, id, value)) {
        msgCtx.commandError(MessageBuilder() << "  Failed to set " << id << " to value \"" << value << "\"");
    }
}

void ItemEditor::handleGet(const std::string &id)
{
    MessageContext &msgCtx = mClient->msgCtx();
    if (!sItemProperties.hasProperty(id)) {
        msgCtx.commandError(MessageBuilder() << "  Can't find property " << id);
        return;
    }

    msgCtx.send(MessageBuilder() << ">> " << sItemProperties.get(mContext, id));
}
