#include "lookcommand.h"
#include "levelservice.h"
#include "characterservice.h"
#include "util/textutils.h"
#include "com/ui.h"

LookCommand::LookCommand() :
    Command("look", "look around/object_name")
{

}

void LookCommand::execute(const CommandContext &c, UI &messageContext) const
{
    const auto &character = c.mCaller;
    Room *r = character->room();
    std::string p = c.mParameters.back();
    text::clean(p);
    text::lowered(p);

    if (p == "around") {
        std::map<Name, int> visibleItems;
        const std::vector<Item*> &items = CS->itemsInVision(character);
        for (Item *item : items) {
            ++visibleItems[item->name()];
        }

        MessageBuilder msg;
        msg << r << ": " << r->description();
        if (!visibleItems.empty()) {
            msg.append(" You see ");
            msg.appendJoin(visibleItems, [](const std::pair<Name, int> &p){
                return p.first.num(p.second, false);
            });
        }
        messageContext.send(msg);
    }
    else {
        messageContext.commandError("Not implemented... Just look around");
    }
}
