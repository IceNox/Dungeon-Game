#pragma once

#include "Navigation.h"

#include "Command.h"

#include "Animation.h"
#include "ScreenAnimations.h"

#include "HealthMap.h"

#include <string>
#include <vector>
#include <chrono>

class Item
{
public:
    std::string name;
    std::string type;
    std::string spriteName;

    int cooldown;
    std::chrono::time_point<std::chrono::system_clock> lastUsed;

    bool activatable;

    Item() {}
    Item(std::string itemName);
    void use_item
    (
        std::vector<DamageMap> &damageMap,
        std::vector<Command> &commands,
        ScreenAnimations &screenAnimations,
        Pos2D gPos,
        Direction facing
    );
};
