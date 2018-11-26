#pragma once

#include "HelperStructs.h"
#include "LevelStateData.h"
#include "Navigation.h"
#include "Constants.h"

#include "HealthMap.h"

#include "Sol.hpp"

#include <vector>

class LevelObject
{
public:
    int oType; // Object type: 0 - static, 1 - dynamic
    int oid;   // a 6-digit number: first 3 digits - ID, last 3 digits - variant
    int ID;
    int VR;

    // Script
    sol::state script;

    // Position
    Pos2D gPos;
    Pos2D cPos;

    Direction facing;

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
    ~LevelObject() = default;

    virtual void update(const LevelStateData &ld, long int curTime) {}
    virtual void interact() {}
    virtual void destroy() {}
    virtual void damage(const DamageInfo &di) {}
    virtual void heal() {}

    void move_to(Pos2D pos, int duration = 0);
    void move_by(Pos2D pos, int duration = 0);
    void move_to_(int x, int y, int duration = 0);
    void move_by_(int x, int y, int duration = 0);
    void update_movement();

private:
    //void self_damage
};

class StaticObject : public LevelObject
{

public:
    StaticObject(int id, Pos2D gPos, bool setup = true);

    virtual void update(const LevelStateData &ld, long int curTime);
    virtual void damage(const DamageInfo &di);
};
