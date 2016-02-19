#include "editor.h"
#include "client.h"
#include "item.h"
#include "resourceservice.h"
#include "propertyserializer.h"
#include <type_traits>

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


#define ADD_ITEM_PROPERTY(_NAME, _TYPE, _GETTER, _SETTER) \
    sItemProperties.addProperty(_NAME, \
        [](Item *base) { return property::to_string(base->_GETTER()); }, \
        [](Item *base, const std::string &value) { return base->_SETTER(property::from_string<_TYPE>(value));})


#define ADD_ITEM_TRAIT_PROPERTY(_NAME, _TRAIT, _TYPE, _GETTER, _SETTER) \
    sItemProperties.addProperty(_NAME, \
        [](Item *base) { return base->hasTrait<_TRAIT>(); }, \
        [](Item *base) { return property::to_string(base->_GETTER()); }, \
        [](Item *base, const std::string &value) { return base->_SETTER(property::from_string<_TYPE>(value));})

void BaseEditor::setupEditors()
{
    ADD_ITEM_PROPERTY("name", Name, name, setName);
    ADD_ITEM_PROPERTY("weight", double, weight, setWeight);
    ADD_ITEM_PROPERTY("size", Box<float>, size, setSize);
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

void ItemEditor::list(ListCommandParameter p)
{
    switch (p) {
    case ListCommandParameter::Properties:
        mClient->msgCtx().send("  Properties: " + text::join(sItemProperties.list(mContext)));
        break;
    case ListCommandParameter::Traits:
        MessageBuilder mb("  Traits: ");
        mb.appendJoin(mItem->traits(), [](const std::pair<const unsigned, std::unique_ptr<ItemTrait> > &idTraitPair) {
            return idTraitPair.second->traitName();
        });
        mClient->msgCtx().send(mb);
        break;
    }
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
    try {
        if (!sItemProperties.set(mContext, id, value)) {
            msgCtx.commandError(MessageBuilder() << "  Can't find property '" << id << "'");
        }
    }
    catch (const property::Exception &e) {
        msgCtx.commandError(MessageBuilder() << "  " << e.mMessage);
    }
}

void ItemEditor::handleGet(const std::string &id)
{
    MessageContext &msgCtx = mClient->msgCtx();
    if (!sItemProperties.hasProperty(mContext, id)) {
        msgCtx.commandError(MessageBuilder() << "  Can't find property '" << id << "'");
        return;
    }

    msgCtx.send(MessageBuilder() << id << ": " << sItemProperties.get(mContext, id));
}
