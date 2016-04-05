#include "lookcommand.h"
#include "levelservice.h"
#include "characterservice.h"
#include "util/textutils.h"
#include "com/ui.h"

LookCommand::LookCommand() :
    Command("look", "look around/<object_name>", 1)
{

}

void LookCommand::execute(const CommandContext &c, UI &ui) const
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
        ui.send(msg);
    }
    else {
        std::string baseName = text::lowered(c.mParameters[0]);
        text::removePrefixes(baseName, {"at ", "for "});
        try {
            auto lookPair = r->lookMap().find_match(baseName);
            ui.send(lookPair.second);
            return;
        }
        catch (SelectorError e) {
            if (e == SelectorError::MultipleMatches) {
                ui.commandError("There are so many things to look at...");
                return;
            }
        }


        baseName = text::parseItemBaseName(baseName);

        Item *item = CS->selectItemInVision(ui, baseName);
        if (item) {
            ui.printDescription(item);
        }
    }
}
