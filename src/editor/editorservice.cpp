#include "editorservice.h"
#include "editor.h"
#include "resourceservice.h"
#include "client.h"

EditorService *ES = 0;
EditorService::EditorService()
{
    ES = this;
    editor::BaseEditor::setupEditors();
}

EditorService::~EditorService()
{
    ES = 0;
}

std::shared_ptr<editor::BaseEditor> EditorService::startEditingItem(Client *c, const std::string &id)
{
    std::unique_ptr<Item> item = RS->itemCopyForEditing(id);
    if (!item) {
        c->msgCtx().commandError(MessageBuilder() << "Can't find item \"" << id << "\"");
        return nullptr;
    }
    std::shared_ptr<editor::BaseEditor> editor = std::make_shared<editor::ItemEditor>(std::move(item), c);
    c->msgCtx().send(MessageBuilder() << "Started editing item \"" << id << "\"");
    c->setEditor(editor);
    return editor;
}
