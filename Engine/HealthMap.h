#pragma once

#include "Location.h"
#include "Constants.h"

#include <string>
#include <vector>
#include <unordered_map>

class HealMap
{
public:
    int playerHeal;
    int enemyHeal;

    bool active;

    HealMap();
};

class DamageMap
{
public:
    int playerDamagePhysical;
    int playerDamageFire;
    int playerDamageWater;
    int playerDamageMagic;
    int enemyDamagePhysical;
    int enemyDamageFire;
    int enemyDamageWater;
    int enemyDamageMagic;
    int playerDamagePhysicalGround;
    int playerDamageFireGround;
    int playerDamageWaterGround;
    int playerDamageMagicGround;
    int enemyDamagePhysicalGround;
    int enemyDamageFireGround;
    int enemyDamageWaterGround;
    int enemyDamageMagicGround;

    Direction direction;

    bool active;

    bool startShake;

    DamageMap();

    void reset();

    int total_damage_non_ground();
    int total_damage_ground();
    int total_player_damage_non_ground();
    int total_player_damage_ground();
    int total_enemy_damage_non_ground();
    int total_enemy_damage_ground();
};

struct DamageInfo
{
    // Damage info
    int amount = 0;
    DamageType type = D_PHYSICAL;
    bool players = false;
    bool enemies = false;
    bool terrain = false;

    Direction dir = NO_DIRECTION;

    // Damage internal effects
    bool ground        = false; // Only affects targets on ground
    bool precise    = false; // Ignores resistances
    bool startShake = false; // Shakes screen

    // Damage effects on target
    int knockbackStr = 0;
    std::vector<std::string> statusEffects;

    // Other
    std::string source = "???";

    DamageInfo() {};
};

struct HealInfo
{
    // Heal info
    int    amount = 0;
    bool players = false;
    bool enemies = false;

    // Other
    std::string source = "???";

    HealInfo() {};
};

class HealthMap
{
    bool active; // Specifies if the tile has any damage/heal applied

    std::vector<DamageInfo> dInfo;
    std::vector<HealInfo>   hInfo;
public:

    HealthMap();

    std::string add_damage(std::string msg, std::string source = "unspecified");
    std::string add_heal  (std::string msg, std::string source = "unspecified");

    void reset();
};
