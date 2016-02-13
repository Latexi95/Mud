#ifndef EDITORSERVICE_H
#define EDITORSERVICE_H
#include <string>
#include <memory>
namespace editor {
    class BaseEditor;
}

class Client;
class EditorService
{
public:
    EditorService();
    ~EditorService();
    std::shared_ptr<editor::BaseEditor> startEditingItem(Client *c, const std::string &id);
};

extern EditorService *ES;
#endif // EDITORSERVICE_H
