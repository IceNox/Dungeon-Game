#pragma once

#include "Constants.h"
#include "HelperStructs.h"
#include "GlobalData.h"
#include "ConversionMaps.h"

#include "LevelMessage.h"

#include "Functions.h"
#include "GameTime.h"
#include "Navigation.h"
#include "HealthMap.h"

#include <windows.h>
#include <string>
#include <vector>

enum EntityType
{
    ENTITY_BASE,
    ENTITY_DAMAGE,
    ENTITY_HEAL,
    ENTITY_GOLD,
    ENTITY_ITEM
};

class Entity
{
public:
    // Position
    Pos2D gPos;
    Pos2D cPos;
    Pos2D cPos_hp;

    // Collision
    bool collidesP = false; // Collides with players
    bool collidesE = false; // Collides with enemies

    Hitbox hitbox;

    // Visuals
    Pos2D sPos;
    Pos2D spriteOffset;

    int spriteIndex = EMPTY_SPRITE;
    int spriteWidth;
    int spriteHeight;
    int spriteCount;

    bool animated;
    int currentFrame;
    int frameHoldTime;
    TimePoint lastFrameChange;

    RECT spriteRegion;

    // Visibility
    bool revealed = false;
    bool visible  = false;

    // Health
    DamageInfo damage;
    HealInfo heal;

    // Functions
    ~Entity() = default;

    virtual void update() {}
    virtual EntityType get_type() { return ENTITY_BASE; }
};

class DamageProjectile : public Entity
{
public:
    Pos2D vVec; // Velocity
    TimePoint lastMove;

    DamageProjectile
    (
        Pos2D cPos,
        Pos2D vVec,
        bool cwp,
        bool cwe,
        DamageInfo di,
        Hitbox hb,
        int spri,
        int holdtime = 150
    );
    DamageProjectile(const LevelMessage &msg, std::string &result);

    virtual void update();
    virtual EntityType get_type() { return ENTITY_DAMAGE; }
};

class HealProjectile : public Entity
{
public:
    Pos2D vVec; // Velocity
    TimePoint lastMove;

    HealProjectile
    (
        Pos2D cPos,
        Pos2D vVec,
        bool cwp,
        bool cwe,
        HealInfo hi,
        Hitbox hb,
        int spri,
        int holdtime = 150
    );
    HealProjectile(const LevelMessage &msg, std::string &result);

    virtual void update();
    virtual EntityType get_type() { return ENTITY_HEAL; }
};

class GoldEntity : public Entity
{
public:
    int amount;

    GoldEntity(Pos2D gPos, int amount);
    GoldEntity(const LevelMessage &msg, std::string &result);

    virtual void update();
    virtual EntityType get_type() { return ENTITY_GOLD; }
};

class ItemEntity : public Entity
{
public:
    virtual void update();
    virtual EntityType get_type() { return ENTITY_ITEM; }
};

/*
class Entity
{
public:
    bool collidesWithPlayer;
    bool collidesWithAI;

    Pos2D gPos;
    Pos2D cPos;
    Pos2D sPos;

    Pos2D vVec;
    Pos2D scPos;
    Pos2D rangeVec;

    Pos2D sOffset;

    bool item;
    bool gold;

    std::string itemName;
    int goldAmount;
    int damage;
    int heal;

    int hitboxRadius;

    bool revealed;
    bool visible;

    bool animated;

    TimePoint creationTime;
    TimePoint lastMove;
    TimePoint lastFrameChange;

    int sHeight;
    int sWidth;

    int frameCount;
    int frameHoldTime;
    int currentFrame;

    SpriteName spriteName;
    RECT       spriteRegion;

    Entity // Constructor for a regular entity
    (
        Pos2D cPos,
        Pos2D vVec,
        Pos2D sOffset,
        Pos2D rangeVec,
        int            damage,
        int            heal,
        int            hitboxRadius,
        bool        cwp,
        bool        cwa,
        bool        animated,
        SpriteName    spriteName,
        int            sW,
        int            sH,
        int            frameCount,
        int            frameHoldTime
    );
    Entity // Constructor for gold entity
    (
        Pos2D gPos,
        int    goldAmount
    );
    Entity::Entity // Constructor for item entity
    (
        Pos2D gPos,
        SpriteName itemName
    );
    void update_entity();
};
*/
