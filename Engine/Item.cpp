#include "Item.h"

Item::Item(const Item &i) : Item(i.iid, i.lastUsed)
{}

Item::Item(int id, TimePoint lastUsed)
{
    // Set essential data
    this->iid = id;
    this->ID = iid / 1000;
    this->VR = iid % 1000;

    // Set up script state
    script.open_libraries(sol::lib::base);
    script.open_libraries(sol::lib::math);

    script.new_usertype<Pos2D>
    (
        "position",
        sol::constructors<Pos2D()>(),
        "x", &Pos2D::x,
        "y", &Pos2D::y
    );

    script.new_usertype<TileData>
    (
        "tileData",
        sol::constructors<>(),
        "pressuredW" , sol::readonly(&TileData::pressuredW),
        "pressuredS" , sol::readonly(&TileData::pressuredS),
        "pressuredG" , sol::readonly(&TileData::pressuredG),
        "pressuredB" , sol::readonly(&TileData::pressuredB),
        "pressured"  , sol::readonly(&TileData::pressured),
        "powered"    , sol::readonly(&TileData::powered),
        "terrain"    , sol::readonly(&TileData::terrain),
        "object"     , sol::readonly(&TileData::object),
        "player"     , sol::readonly(&TileData::player),
        "enemy"      , sol::readonly(&TileData::enemy),
        "directlyLit", sol::readonly(&TileData::directlyLit)
    );

    script.new_usertype<LevelStateData>
    (
        "levelData",
        sol::constructors<>(),
        "width" , sol::readonly(&LevelStateData::width),
        "height", sol::readonly(&LevelStateData::height),
        "tiles" , sol::readonly(&LevelStateData::tiles)
    );

    script.new_usertype<DamageInfo>
    (
        "damageInfo",
        sol::constructors<>(),
        "amount"       , sol::readonly(&DamageInfo::amount),
        "type"         , sol::readonly(&DamageInfo::type),
        "players"      , sol::readonly(&DamageInfo::players),
        "enemies"      , sol::readonly(&DamageInfo::enemies),
        "terrain"      , sol::readonly(&DamageInfo::terrain),
        "direction"    , sol::readonly(&DamageInfo::dir),
        "ground"       , sol::readonly(&DamageInfo::ground),
        "precise"      , sol::readonly(&DamageInfo::precise),
        "knockbackStr" , sol::readonly(&DamageInfo::knockbackStr),
        "statusEffects", sol::readonly(&DamageInfo::statusEffects)
    );

    script.new_usertype<LevelMessage>
    (
        "levelMessage",
        sol::constructors<LevelMessage()>(),
        "source"    , &LevelMessage::source,
        "message"   , &LevelMessage::message,
        "argKeys"   , &LevelMessage::argKeys,
        "argValsInt", &LevelMessage::argValsInt,
        "argValsStr", &LevelMessage::argValsStr
    );

    // Link object to apropriate script
    int linked = false;
    for (int i = 0; i < _ITEM_SCRIPTS.size(); i++) {
        if (id / 1000 == _ITEM_SCRIPTS[i].id) {
            // Open script file
            script.script_file(_ITEM_SCRIPTS[i].filePath);

            // Set sprites
            for (int j = 0; j < _ITEM_SCRIPTS[i].spriteIndexes.size(); j++) {
                script["spriteindexes"][j+1] = _ITEM_SCRIPTS[i].spriteIndexes[j];
            }

            linked = true;
            break;
        }
    }

    if (!linked) return;

    // Stacking
    stackable = script["stackable"];
    stackLimit = script["stacklimit"];

    // Usage
    usable = script["usable"];
    usetime = script["usetime"];
    cooldown = script["cooldown"];

    // Set sprite
    spriteIndex = script["spriteindexes"][1];
}

void Item::use(Pos2D pos, Direction dir, const LevelStateData &ld)
{
    script["use"](pos, dir, ld, maintime::currentGameTime);
}

void Item::update(Pos2D pos, Direction dir, const LevelStateData &ld)
{
    script["update"](pos, dir, ld, maintime::currentGameTime);
}

void Item::get_messages(std::vector<LevelMessage> &messages)
{
    int count = script["messagecount"].get<int>();
    for (int i = 0; i < count; i++) {
        messages.push_back(script["get_message"](i));
    }
}

void Item::clear_messages()
{
    script["clear_messages"]();
}

/*
Item::Item(std::string itemName)
{
    if (itemName == "wooden_sword") {
        name       = "sword";
        type       = "wooden";
        spriteName = "wooden_sword";

        cooldown    = 500;
        activatable = true;
    }
    else if (itemName == "steel_sword") {
        name       = "sword";
        type       = "steel";
        spriteName = "steel_sword";

        cooldown = 500;
        activatable = true;
    }

    lastUsed = system_clock::now() - milliseconds(cooldown);
}

void Item::use_item
(
    std::vector<DamageMap>& damageMap,
    std::vector<Command> &commands,
    ScreenAnimations &screenAnimations,
    Pos2D gPos,
    Direction facing
)
{
    
    if (!activatable) return;

    std::chrono::duration<float> timeElapsed = system_clock::now() - lastUsed;
    if (timeElapsed < milliseconds(cooldown)) return;

    if (name == "sword") {
        int index = gPos.index(_LEVEL_WIDTH);

        if        (facing == UP     && gPos.y > 0                  ) index -= _LEVEL_WIDTH;
        else if (facing == DOWN  && gPos.y < _LEVEL_HEIGHT - 1) index += _LEVEL_WIDTH;
        else if (facing == LEFT  && gPos.x > 0                  ) index -= 1;
        else if (facing == RIGHT && gPos.x < _LEVEL_HEIGHT - 1) index += 1;

        if (type == "wooden") damageMap[index].enemyDamagePhysical = 2;
        if (type == "steel")  damageMap[index].enemyDamagePhysical = 4;

        if        (facing == UP   ) damageMap[index].direction = UP;
        else if (facing == DOWN ) damageMap[index].direction = DOWN;
        else if (facing == LEFT ) damageMap[index].direction = LEFT;
        else if (facing == RIGHT) damageMap[index].direction = RIGHT;

        damageMap[index].active        = true;
        damageMap[index].startShake = true;

        int soX;
        int soY;

        int sprX;
        int sprY = 72;

        if        (facing == "up") {
            soX = 0;
            soY = -132;

            sprX = 8;
        }
        else if (facing == "down") {
            soX = -8;
            soY = -36;

            sprX = 8;
        }
        else if (facing == "left") {
            soX = -68;
            soY = -72;

            sprX = 24;
        }
        else if (facing == "right") {
            soX = 44;
            soY = -72;

            sprX = 24;
        }
        
        //visualEffects.push_back(VisualEffect(x * cellSize + cellSize / 2 + soX, y * cellSize + cellSize / 2 + soY, false, "attack_chop_" + facing + "_effect", sprX, sprY, 0.01f, 7));
        commands.push_back(Command(x * cellSize + cellSize / 2 + soX, y * cellSize + cellSize / 2 + soY, false, "attack_chop_" + facing + "_effect", sprX, sprY, 0.01f, 7, 1.0f));
        
    }

    lastUsed = std::chrono::system_clock::now();
    
}
*/
