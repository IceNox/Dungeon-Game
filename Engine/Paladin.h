#pragma once

#include "AI.h"
#include "Tile.h"
#include "Player.h"
#include "Entity.h"
#include "Particle.h"
#include "HealthMap.h"
#include "Animation.h"

#include <string>
#include <chrono>
#include <vector>

using namespace std::chrono;

class Paladin : public AI
{
public:
    
    std::string type;
    std::string facing;
    
    std::string currentAction;
    std::string spriteFacing;
    std::string spriteNumber;
    
    int hitDamage;
    int chargeDamage;
    int blockDamage;

    float actionCooldown = 1.0f;
    time_point<system_clock> lastAction;
    
    // Movement variables
    float jumpHeight = 30.0f;

    bool changedPosition = false;

    float                     MAlength = 0.2f;
    float                     c_MAlength;
    bool                     inMA = false;
    float                     MAtimeElapsed;
    float                     MAprogress;
    std::string                 MAdirection;
    time_point<system_clock> MAstart;
    int                         sgX;
    int                         sgY;
    
    // Attack variables
    float                     AAlength = 0.5f;
    bool                     inAA = false;
    float                     AAtimeElapsed;
    float                     AAprogress;
    std::string                 AAdirection;
    time_point<system_clock> AAstart;

    // Block variables
    float                     BAcooldown = 5.0f;
    bool                     inBA = false;
    std::string                 BAdirection;
    bool                     blockedDamage = false;
    time_point<system_clock> lastBlock;

    // Charge variables
    float                     CAcooldown = 7.5f;
    float                     CAlength = 0.5f;
    bool                     inCA = false;
    std::string                 CAdirection;
    time_point<system_clock> CAstart;
    time_point<system_clock> lastCharge;
    
    // Functions
    Paladin
    (
        int gX,
        int gY,
        std::string type,
        std::vector<Command> &commands,
        bool spawn = false
    );
    
    void update_paladin
    (
        Player &player,
        std::vector<Tile> &tiles,
        std::vector<DamageMap> &damageMap,
        std::vector<Command> &commands
    );

    void block
    (
        Player &player,
        std::vector<DamageMap> &damageMap,
        std::vector<Command> &commands
    );

    void charge
    (
        Player &player,
        std::vector<Tile> &tiles,
        std::vector<DamageMap> &damageMap,
        std::vector<Command> &commands
    );

    void attack
    (
        Player &player,
        std::vector<DamageMap> &damageMap,
        std::vector<Command> &commands
    );

    void move
    (
        Player &player,
        std::vector<Tile> &tiles
    );

    void end_MA();

    void update_idle_animation();

    void damage
    (
        int amount,
        std::string type,
        Direction direction,
        std::vector<Command> &commands
    );

    void damage
    (
        DamageMap damageMap,
        std::vector<Command> &commands
    );
};