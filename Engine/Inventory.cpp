#include "Inventory.h"

Inventory::Inventory()
{
    items.push_back(Item(1000));
    equipedItems[0] = 0;
    currentlySelected = 0;
}

void Inventory::update(std::vector<LevelMessage> &messages, Pos2D pos, Direction dir, const LevelStateData &ld, kb::Keys &k, UserData &u)
{
    if (k.key_clicked(u.keyBindings.USE_ITEM)) {
        items[equipedItems[currentlySelected]].use(pos, dir, ld);
        items[equipedItems[currentlySelected]].get_messages(messages);
        items[equipedItems[currentlySelected]].clear_messages();
    }
}
