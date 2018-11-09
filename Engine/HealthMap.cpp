#include "HealthMap.h"

#include "Functions.h"
#include "ConversionMaps.h"

#include <exception>

HealthMap::HealthMap()
{
    reset();
}

std::string HealthMap::add_damage(std::string msg, std::string source)
{
    /** Format of damage message:
    * [] - mandatory arguments, <> - optional arguments
    * i - integers, s - strings
    * 
    * >>> i[ amount ] s[ type ] s[ direction ] i[ knockback ] s<arg1>|s<arg2>|...
    *
    * [type]      : "physical", "p", "air", "a", "earth", "e", "fire", "f", "water", "w", "holy", "h", "demonic", "d"
    * [direction] : "up", "down", "left", "right", "none", "u", "d", "l", "r", "n"
    * <arg>          : "ground", "precise", "players", "enemies", "terrain", "effect:*type*;*duration*;*strength*"
    */

    // Get arguments
    std::vector<std::string> args;
    args.reserve(10);

    split_str(msg, args);

    // Return if not enough arguments
    if (args.size() < 4) return "error: not enough arguments";

    dInfo.push_back(DamageInfo());
    int index = dInfo.size() - 1;

    /// Evaluate mandatory arguments
    // Amount
    dInfo[index].amount = str_to_int(args[0]);

    // Type
    try {
        dInfo[index].type = damageTypeMap.at(args[1]);
    }
    catch (...) {
        return "error: invalid type";
    }

    // Direction
    try {
        dInfo[index].dir = directionMap.at(args[2]);
    }
    catch (...) {
        return "error: invalid direction";
    }

    // Knockback
    dInfo[index].knockbackStr = str_to_int(args[3]);
    if (dInfo[index].knockbackStr > 10) dInfo[index].knockbackStr = 10;

    /// Evaluate other arguments
    for (int i = 4; i < args.size(); i++) {
        if        (args[i] == "ground")
            dInfo[index].ground = true;
        else if (args[i] == "precise")
            dInfo[index].precise = true;
        else if (args[i] == "players")
            dInfo[index].players = true;
        else if (args[i] == "player")
            dInfo[index].players = true;
        else if (args[i] == "p")
            dInfo[index].players = true;
        else if (args[i] == "enemies")
            dInfo[index].enemies = true;
        else if (args[i] == "enemy")
            dInfo[index].enemies = true;
        else if (args[i] == "e")
            dInfo[index].enemies = true;
        else if (args[i] == "terrain")
            dInfo[index].terrain = true;
        else if (args[i] == "t")
            dInfo[index].terrain = true;
        else if (args[i].length() > 7)
            if (args[i].substr(0, 7) == "effect:")
                dInfo[index].statusEffects.push_back(args[i].substr(7));
        else
            return "error: argument \"" + args[i] + "\" is invalid";
    }

    if (!dInfo[index].players && !dInfo[index].enemies && !dInfo[index].terrain) {
        dInfo[index].players = true;
        dInfo[index].enemies = true;
        dInfo[index].terrain = true;
    }

    return "success!";
}

std::string HealthMap::add_heal(std::string msg, std::string source)
{
    /** Format of heal message:
    * [] - mandatory arguments, <> - optional arguments
    * i - integers, s - strings
    *
    * >>> s[target] i[amount]
    *
    * [target]: "player", "enemy", "p", "e"
    */

    // Get arguments
    std::vector<std::string> args;
    args.reserve(10);

    split_str(msg, args);

    // Return if not enough arguments
    if (args.size() < 2) return "error: not enough parameters";

    hInfo.push_back(HealInfo());
    int index = hInfo.size() - 1;

    // Evaluate arguments
    hInfo[index].amount = str_to_int(args[0]);

    for (int i = 1; i < args.size(); i++) {
        if        (args[i] == "player")
            hInfo[index].players = true;
        else if (args[i] == "players")
            hInfo[index].players = true;
        else if (args[i] == "p")
            hInfo[index].players = true;
        else if (args[i] == "enemy")
            hInfo[index].enemies = true;
        else if (args[i] == "enemies")
            hInfo[index].enemies = true;
        else if (args[i] == "e")
            hInfo[index].enemies = true;
        else
            return "error: argument \"" + args[i] + "\" is invalid";
    }

    if (!hInfo[index].players && !hInfo[index].enemies) {
        hInfo[index].players = true;
        hInfo[index].enemies = true;
    }

    return "success!";
}

void HealthMap::reset()
{
    // Null all variables
    active = false;

    dInfo.clear();
    hInfo.clear();
}

HealMap::HealMap()
{
    playerHeal = 0;
    enemyHeal  = 0;

    active = false;
}

DamageMap::DamageMap()
{
    playerDamagePhysical        = 0;
    playerDamageFire            = 0;
    playerDamageWater            = 0;
    playerDamageMagic            = 0;
    enemyDamagePhysical            = 0;
    enemyDamageFire                = 0;
    enemyDamageWater            = 0;
    enemyDamageMagic            = 0;
    playerDamagePhysicalGround    = 0;
    playerDamageFireGround        = 0;
    playerDamageWaterGround        = 0;
    playerDamageMagicGround        = 0;
    enemyDamagePhysicalGround    = 0;
    enemyDamageFireGround        = 0;
    enemyDamageWaterGround        = 0;
    enemyDamageMagicGround        = 0;

    direction  = NO_DIRECTION;
    active       = false;
    startShake = false;
}

void DamageMap::reset()
{

    playerDamagePhysical        = 0;
    playerDamageFire            = 0;
    playerDamageWater            = 0;
    playerDamageMagic            = 0;
    enemyDamagePhysical            = 0;
    enemyDamageFire                = 0;
    enemyDamageWater            = 0;
    enemyDamageMagic            = 0;
    playerDamagePhysicalGround    = 0;
    playerDamageFireGround        = 0;
    playerDamageWaterGround        = 0;
    playerDamageMagicGround        = 0;
    enemyDamagePhysicalGround    = 0;
    enemyDamageFireGround        = 0;
    enemyDamageWaterGround        = 0;
    enemyDamageMagicGround        = 0;

    direction    = NO_DIRECTION;
    active        = false;
    startShake    = false;
}

int DamageMap::total_damage_non_ground()
{
    return
        playerDamagePhysical +
        playerDamageFire +
        playerDamageWater +
        playerDamageMagic +
        enemyDamagePhysical +
        enemyDamageFire +
        enemyDamageWater +
        enemyDamageMagic
    ;
}

int DamageMap::total_damage_ground()
{
    return
        playerDamagePhysicalGround +
        playerDamageFireGround +
        playerDamageWaterGround +
        playerDamageMagicGround +
        enemyDamagePhysicalGround +
        enemyDamageFireGround +
        enemyDamageWaterGround +
        enemyDamageMagicGround
    ;
}

int DamageMap::total_player_damage_non_ground()
{
    return
        playerDamagePhysical +
        playerDamageFire +
        playerDamageWater +
        playerDamageMagic
    ;
}

int DamageMap::total_player_damage_ground()
{
    return
        playerDamagePhysicalGround +
        playerDamageFireGround +
        playerDamageWaterGround +
        playerDamageMagicGround
    ;
}

int DamageMap::total_enemy_damage_non_ground()
{
    return
        enemyDamagePhysical +
        enemyDamageFire +
        enemyDamageWater +
        enemyDamageMagic
    ;
}

int DamageMap::total_enemy_damage_ground()
{
    return
        enemyDamagePhysicalGround +
        enemyDamageFireGround +
        enemyDamageWaterGround +
        enemyDamageMagicGround
    ;
}