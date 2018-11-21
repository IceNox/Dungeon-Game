#pragma once

#include "HelperStructs.h"
#include "LevelStateData.h"
#include "Location.h"
#include "Constants.h"

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
    
    // Movement
    int jumpHeight   = 30;
    int moveDuration = 150;

    // Movement state
    ActionState moveState;

    Pos2D moveStartPos;
    Pos2D moveEndPos;

    int moveHeight;

    Direction moveDirection;

    // FUNCTIONS
    ~LevelObject() = default;

    virtual void update(const LevelStateData &ld, long int curTime) {}
    virtual void interact() {}
    virtual void destroy() {}
    virtual void damage() {}
    virtual void heal() {}

    void move(Pos2D pos, int duration = 0, int jumpHeight = 0);
    void update_movement();
};

class StaticObject : public LevelObject
{

public:
    StaticObject(int id, Pos2D gPos);

    virtual void update(const LevelStateData &ld, long int curTime);
};
