#pragma once

#include "Navigation.h"
#include "GameTime.h"

#include "LevelStateData.h"

#include <string>
#include <vector>

enum GateType
{
    DIODE,
    AND,
    OR,
    NOT,
    XOR
};

class PowerGrid
{
    class WireTile
    {
    public:
        std::vector<int> types;
        std::vector<bool> powered;

        WireTile() {}
    };

    class GateTile
    {
    public:
        Pos2D pos;
        GateType type;
        Direction facing;

        bool poweredL; // Left
        bool poweredR; // Right
        bool poweredB; // Behind

        GateTile(Pos2D pos, GateType type, Direction facing, bool poweredL, bool poweredR, bool poweredB)
            : pos(pos), type(type), facing(facing), poweredL(poweredL), poweredR(poweredR), poweredB(poweredB) {}
    };

    std::vector<WireTile> wireGrid;
    std::vector<GateTile> gates;

    int width;
    int height;

    // Timer
    TimePoint lastUpdate = maintime::now();
    unsigned tickCount = 0u;
    const int MS_PER_TICK = 50;

    // Functions
    void power(int index, Direction dir, int type = -1);

public:
    void update(const std::vector<TileData>& td);

    PowerGrid();
    void set_dimensions(int width, int height);
    void set_wire(Pos2D pos, int type, bool powered);
    void set_gate(Pos2D pos, GateType type, Direction facing, bool poweredL, bool poweredR, bool poweredB);
    bool is_powered(Pos2D pos) const;
};
