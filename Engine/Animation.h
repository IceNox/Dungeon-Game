#pragma once

#include "GameTime.h"

#include "Position.h"

#include <windows.h>
#include <string>
#include <chrono>

using namespace std::chrono;

class Animation
{
public:
    Pos2D cPos;

    bool ground;

    SpriteName spriteName;
    RECT       spriteRegion;
    
    int sWidth;
    int sHeight;

    int frameHoldTime;
    int    currentFrame;
    int    frameCount;

    TimePoint creationTime;

    bool finished;

    Animation
    (
        Pos2D cPos,
        bool g,
        SpriteName name,
        int sw,
        int sh,
        int ht,
        int fc
    );

    void update_animation();
};