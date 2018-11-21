#pragma once

#include "GameTime.h"
#include "Location.h"
#include "Constants.h"

#include <Windows.h>

class Portal
{
public:
    Pos2D entryPos;
    Pos2D exitPos;
    int newLevel;

    bool exit;

    bool revealed = false;
    bool visible;

    // Animation variables
    const int frameCount    = 3;
    const int frameHoldTime = 150;
    int    currentFrame;

    TimePoint lastFrameChange;

    RECT spriteRegion;

    // Functions
    Portal() {}
    Portal(Pos2D entryP, Pos2D exitP, int nl);

    void update_portal();
};
