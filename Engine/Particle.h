#pragma once

#include "GameTime.h"

#include "Location.h"

#include "Sprite.h"

#include <string>

class Particle
{
public:
    Pos2D cPos;     // Position
    Pos2D rcPos; // High precision position

    Sprite s;

    bool isText = false;
    std::string text;

    Pos2D vVec;     // Initial velocity vector
    Pos2D rvVec; // High precision initial velocity vector

    Pos2D aVec;  // Acceleration vector

    TimePoint creationTime;
    TimePoint lastUpdated;

    int existenceLength;
    int fadeLength;

    float transparency;

    bool finished;

    Particle
    (
        Pos2D cPos,
        Pos2D vVec,
        Pos2D aVec,
        Sprite s,
        int eLength,
        int fLength
    );

    Particle
    (
        Pos2D cpos,
        std::string text
    );

    void update_particle();
};
