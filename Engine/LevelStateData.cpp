#include "LevelStateData.h"

namespace lsd
{
    void reset(LevelStateData &ld)
    {
        for (int i = 0; i < ld.tiles.size(); i++) {
            ld.tiles[i].pressuredW = false;
            ld.tiles[i].pressuredS = false;
            ld.tiles[i].pressuredG = false;
            ld.tiles[i].pressuredB = false;
            ld.tiles[i].powered = false;

            ld.tiles[i].occupied = false;
            ld.tiles[i].terrain = false;
            ld.tiles[i].object = false;

            ld.tiles[i].player = false;
            ld.tiles[i].enemy = false;

            ld.tiles[i].playerId = -1;
            ld.tiles[i].enemyId = -1;

            ld.tiles[i].directlyLit = false;
        }

        ld.playerCount = 0;
        ld.playerX.clear();
        ld.playerY.clear();
    }

    void set(LevelStateData &ld, int w, int h)
    {
        ld.width = w;
        ld.height = h;

        ld.tiles.clear();
        for (int i = 0; i < w * h; i++) {
            ld.tiles.push_back(TileData());
        }

        reset(ld);
    };
}
