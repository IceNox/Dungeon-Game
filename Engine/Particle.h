#pragma once

#include "GameTime.h"
#include "Navigation.h"

#include "LevelMessage.h"

#include "Sprite.h"

#include <string>

enum ParticleDisplayType
{
    PARTICLE_SPRITE,
    PARTICLE_RECTANGLE,
    PARTICLE_TEXT
};

class Particle
{
public:
    Pos2D cPos;     // Position
    Pos2D cPos_hp; // High precision position

    Pos2D vVec;     // Initial velocity vector
    Pos2D vVec_hp; // High precision initial velocity vector

    Pos2D aVec; // Acceleration vector


    ParticleDisplayType dtype = PARTICLE_TEXT;

    // Type sprite
    int spriteIndex = EMPTY_SPRITE;

    // Type rectangle
    Sprite srect = Sprite(1, 1, "", 0, 0, 0);

    // Type text
    std::string text;

    // Type sprite/rectangle
    Pos2D sPos;
    Pos2D spriteOffset;
    int spriteWidth;
    int spriteHeight;

    Color rtColor; // Rectangle/Text color


    TimePoint creationTime;
    TimePoint lastUpdated;

    int existenceLength;
    int fadeLength;
    bool fading = true;

    float transparency;

    bool finished;

    Particle::Particle
    (
        Pos2D cPos,
        Pos2D vVec,
        Pos2D aVec,
        int spri,
        int eLength,
        int fLength,
        bool fading = true
    );

    Particle
    (
        Pos2D cPos,
        Pos2D vVec,
        Pos2D aVec,
        Sprite s,
        int eLength,
        int fLength,
        bool fading = true
    );

    Particle
    (
        Pos2D cpos,
        std::string text,
        Color c,
        bool fading = true
    );

    Particle(const LevelMessage &msg, std::string &result);

    void update_particle();
};
