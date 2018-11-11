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
    void set(LevelStateData &ld, int w, int h)
    {
        ld.width  = w;
        ld.height = h;

        ld.tiles.clear();
        for (int i = 0; i < w * h; i++) {
            ld.tiles.push_back(TileData());
        }

        reset(ld);
    };

    void reset(LevelStateData &ld)
    {
        for (int i = 0; i < ld.tiles.size(); i++) {
            ld.tiles[i].pressuredW = false;
            ld.tiles[i].pressuredS = false;
            ld.tiles[i].pressuredG = false;
            ld.tiles[i].pressuredB = false;
            ld.tiles[i].powered    = false;

            ld.tiles[i].terrain = false;
            ld.tiles[i].object  = false;

            ld.tiles[i].player = false;
            ld.tiles[i].enemy  = false;

            ld.tiles[i].directlyLit = false;
        }
    }
}
