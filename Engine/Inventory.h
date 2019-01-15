#pragma once

#include "Item.h"
#include "UserData.h"

class Inventory
{
public:
    // Inventory
    std::vector<Item> items;

    // Equiped
    int currentlySelected;
    int equipedItems[3];


    Inventory();
    void update(std::vector<LevelMessage> &messages, Pos2D pos, Direction dir, const LevelStateData &ld, Key &k, UserData &u);
};
