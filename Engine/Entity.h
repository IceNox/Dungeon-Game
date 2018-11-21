#pragma once

#include "Constants.h"

#include "GameTime.h"
#include "Navigation.h"

#include <windows.h>
#include <string>
#include <vector>

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
