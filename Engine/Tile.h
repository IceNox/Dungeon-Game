#pragma once

#include "Constants.h"

#include "GameTime.h"

#include "Wire.h"

#include "HealthMap.h"

#include <string>
#include <vector>

class Tile
{
public:
    bool isSet;

    Pos2D gridPos;
    Pos2D centerPos;

    TileType type;
    TileVariant variant;
    bool elevated;
    bool pressuredW;
    bool pressuredS;
    bool pressuredG;
    bool pressuredB;
    bool pressured;
    bool occupied;
    bool powered;

    bool hasDoor;
    bool burning;

    bool activatable;
    bool toggleable;
    bool active;

    bool wasPoweredOff;

    bool transparent;

    bool directlyLit;

    int      lightLevel;
    float brightness;
    bool  visible;
    bool  revealed;

    int          targetLightLevel;
    int          startLightLevel;
    TimePoint lastLightLevelChange;
    float      lightLevelChangeProgress;

    float sideBrightness;
    bool  sideVisible;
    bool  sideRevealed;

    bool                    animated;
    int                        frameCount;
    std::vector<SpriteName> spriteNames;
    int                        currentFrame;
    int                        frameHoldTime;
    TimePoint                lastFrameChange;

    Tile()
    {
        isSet = false;
    }
    void set_tile
    (
        Pos2D        gridPos,
        TileType    type,
        TileVariant variant,
        bool        activatable,
        bool        toggleable,
        bool        active,
        int            frameCount,
        SpriteName    spriteNames[],
        int            frameHoldTime
    );
    void set_tile
    (
        Pos2D        gridPos,
        TileType    type,
        TileVariant variant,
        bool        activatable,
        bool        toggleable,
        bool        active
    );
    void set_light_level
    (
        int lightLevel
    );
    void update_tile
    (
        std::vector<Wire> wires,
        TileVariant baseFloorVariant,
        std::vector<DamageMap> &damageMap
    );
};