#pragma once

#include "Command.h"

#include "AI.h"
#include "Player.h"
#include "Entity.h"
#include "Particle.h"
#include "HealthMap.h"
#include "Animation.h"

#include <string>
#include <chrono>
#include <vector>

using namespace std::chrono;

class Zombie : public AI
{
public:
    std::string type;
    std::string facing;

    std::string currentAction;
    std::string spriteFacing;
    std::string spriteNumber;

    int hitDamage;

    float actionCooldown = 1.0f;
    time_point<system_clock> lastAction;

    float baseActionCooldown = 0.5f;
    float ACDMP; // Action cooldown multiplier

    // Movement animation variables
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

    // Attack animation variables
    float                     AAlength = 0.5f;
    bool                     inAA = false;
    float                     AAtimeElapsed;
    float                     AAprogress;
    std::string                 AAdirection;
    time_point<system_clock> AAstart;

    Zombie
    (
        int gX,
        int gY,
        std::string type,
        std::vector<Command> &commands,
        bool spawn = false
    );
    //Zombie(const Zombie&) = default;
    //Zombie& operator=(const Zombie&) = default;
private:
    void spawn();

public:
    void update_zombie
    (
        Player &player,
        std::vector<Tile> &tiles,
        std::vector<DamageMap> &damageMap,
        std::vector<Command> &commands,
        std::vector<Zombie> &zombies
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
    void update_ACDMP
    (
        Player &player,
        std::vector<Zombie> &zombies
    );

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