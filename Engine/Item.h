#pragma once

#include "Constants.h"
#include "HelperStructs.h"
#include "LevelStateData.h"
#include "GlobalData.h"
#include "ConversionMaps.h"

#include "LevelMessage.h"

#include "Functions.h"
#include "GameTime.h"
#include "Navigation.h"
#include "HealthMap.h"

#include "Sol.hpp"

#include <string>
#include <vector>
#include <chrono>

class Item
{
public:
    int iid; // a 6-digit number: first 3 digits - ID, last 3 digits - variant
    int ID;
    int VR;

    // Script
    sol::state script;

    // Stacking
    bool stackable;
    bool stackLimit;

    // Usage
    bool usable;
    int usetime;
    int cooldown; // in ms
    
    TimePoint lastUsed;

    // Visuals
    int spriteIndex;

    // FUNCTIONS
    Item(const Item &i);
    Item(int id, TimePoint lastUsed = 0);

    void use(Pos2D pos, Direction dir, const LevelStateData &ld);
    void update(Pos2D pos, Direction dir, const LevelStateData &ld);
    void get_messages(std::vector<LevelMessage> &messages);
    void clear_messages();

    /*
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
    */
};
