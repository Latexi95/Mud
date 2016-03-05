#include "editor.h"
#include "client.h"
#include "item.h"
#include "resourceservice.h"
#include "propertyserializer.h"
#include "traits/eatabletrait.h"
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
        [](Item *base) { return property::to_string(base->trait<_TRAIT>()->_GETTER()); }, \
        [](Item *base, const std::string &value) { return base->trait<_TRAIT>()->_SETTER(property::from_string<_TYPE>(value));})

void BaseEditor::setupEditors()
{
    ADD_ITEM_PROPERTY("name", Name, name, setName);
    ADD_ITEM_PROPERTY("weight", double, weight, setWeight);
    ADD_ITEM_PROPERTY("size", Box<float>, size, setSize);

    ADD_ITEM_TRAIT_PROPERTY("eatable.energy", EatableTrait, int, energy, setEnergy);
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

void ItemEditor::add(AddType add, const std::string &id)
{
    switch (add) {
    case AddType::Trait: {
        ItemTraitType type = ItemTraitTypeFromString(id);
        if (type == ItemTraitType::Invalid) {
            mClient->ui().commandError(MessageBuilder("  ") << id << " is not a trait");
            return;
        }
        if (mItem->hasTrait(type)) {
            mClient->ui().commandError(MessageBuilder("  Item already has that trait"));
            return;
        }
        mItem->addTrait(ItemTrait::createItemTraitByType(type));
        mClient->ui().send(MessageBuilder("  Added trait ") << ItemTraitTypeToString(type));
        break;
    }
    default:
        mClient->ui().commandError("  Can't add that to an item");
        break;
    }
}

void ItemEditor::remove(AddType add, const std::string &id)
{
    switch (add) {
    case AddType::Trait: {
        ItemTraitType type = ItemTraitTypeFromString(id);
        if (type == ItemTraitType::Invalid) {
            mClient->ui().commandError(MessageBuilder("  ") << id << " is not a trait");
            return;
        }
        if (!mItem->hasTrait(type)) {
            mClient->ui().commandError(MessageBuilder("  Item doesn't have that trait"));
            return;
        }
        mItem->removeTrait(type);
        mClient->ui().send(MessageBuilder("  Removed trait ") << ItemTraitTypeToString(type));
        break;
    }
    default:
        mClient->ui().commandError("  Can't remove that from an item");
        break;
    }
}

void ItemEditor::list(ListCommandParameter p)
{
    switch (p) {
    case ListCommandParameter::Properties:
        mClient->ui().send("  Properties: " + text::joinWithAnd(sItemProperties.list(mContext)));
        break;
    case ListCommandParameter::Traits:
        MessageBuilder mb("  Traits: ");
        mb.appendJoin(mItem->traits(), [](const std::pair<const unsigned, std::unique_ptr<ItemTrait> > &idTraitPair) {
            return idTraitPair.second->traitName();
        });
        mClient->ui().send(mb);
        break;
    }
}

void ItemEditor::answer(Answer a)
{
    switch (mState) {
    case State::Main:
        mClient->ui().commandError("Ignored");
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
    UI &msgCtx = mClient->ui();
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
    UI &msgCtx = mClient->ui();
    if (!sItemProperties.hasProperty(mContext, id)) {
        msgCtx.commandError(MessageBuilder() << "  Can't find property '" << id << "'");
        return;
    }

    msgCtx.send(MessageBuilder() << id << ": " << sItemProperties.get(mContext, id));
}
