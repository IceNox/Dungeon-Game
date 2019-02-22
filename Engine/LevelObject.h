#pragma once

#include "HelperStructs.h"
#include "LevelStateData.h"
#include "Navigation.h"
#include "Constants.h"
#include "GlobalData.h"

#include "HealthMap.h"

#include "Sol.hpp"

#include <vector>

enum ObjectType
{
    OBJECT_BASE = -1,
    OBJECT_STATIC,
    OBJECT_DYNAMIC
};

class LevelObject
{
public:
    int oid;   // a 6-digit number: first 3 digits - ID, last 3 digits - variant
    int ID;
    int VR;

    // Script index
    int scri;

    // Position
    Pos2D gPos;
    Pos2D cPos;

    Direction facing;

    // Health
    int maxHealth;
    int health;
    int armor;

    int healthbarHeight;
    bool healthbarVisible = false;

    // Sprite
    Pos2D sPos;
    Pos2D sOffset;

    int currentSprite;

    // Hitbox
    Hitbox hitbox;
    Pos2D hOffset;

    // Visibility
    bool revealed = false;
    bool visible  = false;

    // Light
    bool lightSource = false;
    std::vector<Pos2D> lightSourcePos;
    std::vector< int > lightSourceStr;

    // Other
    bool transparent = false;
    bool obstructive = false;
    bool pressuring  = false;
    bool powering    = false;

    // Destruction
    bool destroyed = false;

    // Movement state
    ActionState moveState;

    Pos2D moveStartPos;
    Pos2D moveEndPos;

    int height = 0;

    Direction moveDirection;

    // FUNCTIONS
    LevelObject() = default;

    virtual ObjectType get_type() { return OBJECT_BASE; }

    virtual void update(const LevelStateData &ld, long int curTime) {}
    virtual void interact() {}
    virtual void destroy() {}
    virtual void damage(const DamageInfo &di) {}
    virtual void heal() {}

    void move_to(Pos2D pos, int duration = 0);
    void move_by(Pos2D pos, int duration = 0);
    void move_to_(int x, int y, int duration = 0);
    void move_by_(int x, int y, int duration = 0);
    void update_movement(const LevelStateData &ld);

private:
    //void self_damage
};

class StaticObject : public LevelObject
{
public:
    StaticObject(int id, Pos2D gPos, bool setup = true);

    virtual ObjectType get_type() { return OBJECT_STATIC; }

    virtual void update(const LevelStateData &ld, long int curTime);
    virtual void damage(const DamageInfo &di);
    virtual void destroy();
};

class DynamicObject : public LevelObject
{
public:
    DynamicObject(int id, Pos2D gPos);

    virtual ObjectType get_type() { return OBJECT_DYNAMIC; }

    virtual void update(std::vector<LevelMessage> &messages, const LevelStateData &ld, long int curtime);
    virtual void damage(std::vector<LevelMessage> &messages, const DamageInfo &di);
    virtual void destroy();
};
