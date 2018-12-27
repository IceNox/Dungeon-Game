#pragma once

#include "GameTime.h"

#include "Navigation.h"
#include "Constants.h"

#include "LevelMessage.h"

#include <windows.h>
#include <string>
#include <chrono>

class Animation
{
public:
    Pos2D cPos;

    bool ground;
    bool finished;

    // Visuals
    Pos2D sPos;
    Pos2D spriteOffset;

    int spriteIndex = EMPTY_SPRITE;
    int spriteWidth;
    int spriteHeight;

    int frameCount;
    int currentFrame;
    int frameHoldTime = 150;
    TimePoint creationTime;

    RECT spriteRegion;


    Animation
    (
        Pos2D cPos,
        bool ground,
        int spri,
        int holdtime
    );
    Animation(const LevelMessage &msg, std::string &result);

    void update_animation();
};
