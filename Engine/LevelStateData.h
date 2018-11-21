#pragma once

#include <vector>
#include <string>

struct TileData
{
    // Lua doesn't work as intended with bools
    // Just assume that all ints here are bools

    // Wire stuff
    int pressuredW;
    int pressuredS;
    int pressuredG;
    int pressuredB;
    int pressured;
    int powered;
    
    // Occupation
    int terrain;
    int object;

    //
    int player;
    int enemy;

    int directlyLit;
};

struct LevelStateData
{
    int width;
    int height;

    std::vector<TileData> tiles;
};

namespace lsd
{
    void reset(LevelStateData &ld);
    void set(LevelStateData &ld, int w, int h);
}
