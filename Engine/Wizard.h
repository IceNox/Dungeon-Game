#pragma once

#include "Constants.h"

#include "Command.h"

#include "AI.h"
#include "Player.h"
#include "Entity.h"
#include "Particle.h"
#include "HealthMap.h"
#include "Animation.h"
#include "ScreenAnimations.h"

#include <string>
#include <chrono>
#include <vector>

using namespace std::chrono;

class Wizard : public AI
{
public:
    std::string type;
    std::string facing;

    std::string currentAction;
    std::string spriteFacing;
    std::string spriteNumber;

    float actionCooldown = 1.0f;
    time_point<system_clock> lastAction;

    int boltDamage;
    int eruptionDamage;

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

/// SPELLS
    // Bolt
    float                     boltCooldown = 5.0f;
    float                     BAlength = 0.5f;
    bool                     inBA = false;
    float                     BAtimeElapsed;
    float                     BAprogress;
    std::string                 BAdirection;
    time_point<system_clock> BAstart;
    time_point<system_clock> BAlastCast;

    // Eruption
    float                     eruptionCooldown = 5.0f;
    float                     EAlength = 0.5f;
    bool                     inEA = false;
    float                     EAtimeElapsed;
    float                     EAprogress;
    std::string                 EAdirection;
    time_point<system_clock> EAstart;
    time_point<system_clock> EAlastCast;
    int                         egX;
    int                         egY;

    // Slow
    float                     slowCooldown = 10.0f;
    float                     SAlength = 0.5f;
    bool                     inSA = false;
    float                     SAtimeElapsed;
    float                     SAprogress;
    std::string                 SAdirection;
    time_point<system_clock> SAstart;
    time_point<system_clock> SAlastCast;

    Wizard
    (
        int gX,
        int gY,
        std::string type,
        std::vector<Command> &commands,
        bool spawn = false
    );

    void update_wizard
    (
        Player &player,
        std::vector<Tile> &tiles,
        std::vector<DamageMap> &damageMap,
        std::vector<Command> &commands
    );

    void attack
    (
        Player &player,
        std::vector<Tile> &tiles,
        std::vector<DamageMap> &damageMap,
        std::vector<Command> &commands
    );

    void cast_bolt
    (
        Player &player,
        std::vector<Tile> &tiles,
        std::vector<Command> &commands
    );

    void cast_slow
    (
        Player &player,
        std::vector<Command> &commands
    );

    void cast_eruption
    (
        Player &player,
        std::vector<DamageMap>& damageMap,
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