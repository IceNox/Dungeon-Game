#pragma once

#include "Constants.h"

#include "Tile.h"
#include "ScreenAnimations.h"
#include "Animation.h"
#include "StatusEffects.h"

#include "Inventory.h"

#include "Keys.h"
#include "KeyBindings.h"

#include "UserData.h"

#include <string>
#include <chrono>
#include <vector>

using namespace std::chrono;

class Player
{
public:
    int health;
    int armor;
    int gold;

    int maxHealth;

    float resistancePhysical;
    float resistanceFire;
    float resistanceWater;
    float resistanceMagic;

    int gX, gY, cX, cY, sX, sY;

    Pos2D gPos; // Grid position
    Pos2D cPos; // Pixel position where (0,0) is the top left corner of a map
    Pos2D pPos; // Game pixel position. Game pixel can be a square 4 or 8 pixels in size

    Pos2D spriteOffset;
    Pos2D sPos; // Sprite Position on screen

    int hitboxRadius;

    bool dead;

    bool enterIsPressed  = false;
    bool escapeIsPressed = false;
    bool upIsPressed     = false;
    bool downIsPressed     = false;
    bool leftIsPressed     = false;
    bool rightIsPressed  = false;
    bool spaceIsPressed  = false;

    bool moveBufferUp     = false;
    bool moveBufferDown  = false;
    bool moveBufferLeft  = false;
    bool moveBufferRight = false;

    int lightLevel = 8;

    bool floating;
    bool onGround;

    float damageCooldown = 0.5f;
    time_point<system_clock> lastTookDamage;

    time_point<system_clock> lastAction;

    // Status effects
    StatusEffects statusEffects;

    // Inventory
    Inventory inventory;

    // Sprite variables
    Direction facing;
    std::string skin;

    //Sprite baseDirectionArrowSprite;
    
    Direction spriteFacing;
    int spriteNumber;
    
    PlayerAction currentAction;

    SpriteName finalSprite;

    // Variables for shake animation
    bool tookDamage;
    int damageAmount;

    // Movement animation variables
    float jumpHeight = 30.0f; // in pixels

    bool changedPosition = false;

    const int BASE_MA_LENGTH = 200;
    int MAlength = 200;
    int c_MAlength;
    bool inMA = false;
    float MAprogress;
    Pos2D scPos;
    Pos2D ecPos;
    Pos2D mcVec;
    TimePoint MAstart;
    Direction MAdirection;

    Player
    (
        Pos2D gPos
    );

    void update_player
    (
        std::vector<Tile> tiles,
        std::vector<DamageMap> &damageMap,
        std::vector<Command> &commands,
        std::vector<LevelMessage> &messages,
        ScreenAnimations &screenAnimations,
        const LevelStateData &ld,
        kb::Keys &keys,
        UserData &userData,
        bool lockInput = false
    );

    void get_input
    (
        kb::Keys &keys,
        UserData &userData,
        bool &enter,
        bool &escape,
        bool &up,
        bool &down,
        bool &left,
        bool &right,
        bool &useItem,
        bool &dropItem
    );

    void apply_effects
    (
        std::vector<Command> &commands
    );

    void move
    (
        kb::Keys &keys,
        UserData &userData,
        bool up,
        bool down,
        bool left,
        bool right,
        bool space,
        std::vector<Tile> tiles,
        std::vector<DamageMap>& damageMap,
        std::vector<Command> &commands
    );

    void end_MA();

    void use_items
    (
        bool useItem,
        std::vector<DamageMap>& damageMap,
        std::vector<Command> &commands,
        ScreenAnimations &screenAnimations
    );

    void heal
    (
        int amount
    );

    void damage
    (
        int                     amount,
        DamageType             type,
        Direction             direction,
        std::vector<Command> &commands
    );

    void damage
    (
        DamageMap damageMap,
        std::vector<Command> &commands
    );
};
