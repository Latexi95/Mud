#ifndef EDITOR_H_
#define EDITOR_H_
#include <string>
#include <memory>
#include "properties.h"
#include "editorcommands.h"

class Item;
class Client;

namespace editor {

enum class TryCloseResult {
    Closed,
    DelayedClose,
    CantClose
};

class BaseEditor {
public:
    BaseEditor(Client *client);
    virtual ~BaseEditor();
    virtual TryCloseResult tryClose() = 0;

    virtual void add(AddType add, const std::string &id) = 0;
    virtual void remove(AddType type, const std::string &id) = 0;
    virtual void list(ListCommandParameter p) = 0;
    virtual void answer(Answer a) = 0;
    virtual void saveAndQuit() = 0;
    virtual void quit() = 0;
    virtual void handleSet(const std::string &id, const std::string &value) = 0;
    virtual void handleGet(const std::string &id) = 0;

    Client *client() const;

    static void setupEditors();
    std::shared_ptr<BaseEditor> queuedEditor() const;
    void setQueuedEditor(const std::shared_ptr<BaseEditor> &queuedEditor);

protected:
    Client *mClient;
    std::shared_ptr<BaseEditor> mQueuedEditor;
};

class ItemEditor : public BaseEditor {
public:
    ItemEditor(std::unique_ptr<Item> &&item, Client *client);

    virtual TryCloseResult tryClose();

    virtual void add(AddType add, const std::string &id);
    virtual void remove(AddType type, const std::string &id);
    virtual void list(ListCommandParameter p);
    virtual void answer(Answer a);
    virtual void saveAndQuit();
    virtual void quit();
    virtual void handleSet(const std::string &id, const std::string &value);
    virtual void handleGet(const std::string &id);
private:
    enum class State {
        Main,
        QuitQuestion
    };

    std::unique_ptr<Item> mItem;
    State mState;
    Context<Item> mContext;
};

}


#endif
