#include "Tile.h"

#include "GlobalData.h"

#include "Functions.h"

void Tile::set_tile(
        Pos2D        gridPos,
        TileType    type,
        TileVariant variant,
        bool        activatable,
        bool        toggleable,
        bool        active,
        int            frameCount,
        SpriteName    spriteNames[],
        int            frameHoldTime
    )
{
    this->gridPos        = gridPos;
    this->type            = type;
    this->variant        = variant;
    this->activatable    = activatable;
    this->toggleable    = toggleable;
    this->active        = active;
    this->frameCount    = frameCount;
    this->frameHoldTime = frameHoldTime;

    centerPos = gridPos * cellSize + Pos2D(cellSize / 2);

    //kloooookkkk (this was written by my cat)
    pressuredW = false;
    pressuredS = false;
    pressuredG = false;
    pressured  = false;
    occupied   = false;
    powered    = false;

    hasDoor = false;

    wasPoweredOff = false;

    lightLevel    = 0;
    brightness    = 0.0f;
    visible        = false;
    revealed    = false;

    targetLightLevel = 0;
    startLightLevel  = 0;
    lastLightLevelChange = maintime::now();

    sideBrightness    = 0.0f;
    sideVisible        = false;
    sideRevealed    = false;

    animated = true;

    for (int i = 0; i < frameCount; i++) {
        this->spriteNames.push_back(spriteNames[i]);
    }

    //this->variant   = spriteNames[0];
    currentFrame    = 0;
    lastFrameChange = maintime::now();

    isSet = true;
}

void Tile::set_tile(
        Pos2D        gridPos,
        TileType    type,
        TileVariant variant,
        bool        activatable,
        bool        toggleable,
        bool        active
    )
{
    this->gridPos        = gridPos;
    this->type            = type;
    this->variant        = variant;
    this->activatable    = activatable;
    this->toggleable    = toggleable;
    this->active        = active;

    centerPos = gridPos * cellSize + Pos2D(cellSize / 2);

    //kloooookkkk (this was written by my cat)
    pressuredW = false;
    pressuredS = false;
    pressuredG = false;
    pressured  = false;
    occupied   = false;
    powered    = false;

    hasDoor = false;
    burning = false;

    wasPoweredOff = false;

    lightLevel    = 0;
    brightness    = 0.0f;
    visible        = false;
    revealed    = false;

    targetLightLevel = 0;
    startLightLevel  = 0;
    lastLightLevelChange = maintime::now();

    sideBrightness    = 0.0f;
    sideVisible        = false;
    sideRevealed    = false;

    animated = false;

    isSet = true;
}

void Tile::set_light_level
(
    int lightLevel
)
{
    if (targetLightLevel == lightLevel)
        return;

    add_bounds(lightLevel, 0, 768);

    targetLightLevel     = lightLevel;
    startLightLevel         = this->lightLevel;
    lastLightLevelChange = maintime::now();
}

void Tile::update_tile
(
    const PowerGrid& powerGrid,
    TileVariant baseFloorVariant,
    std::vector<DamageMap> &damageMap
)
{
    // Set transparency
    transparent = true;
    if (type == WALL && active) transparent = false;

    // Update light level
    {
        //std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - lastLightLevelChange;
        
        lightLevelChangeProgress = (maintime::now() - lastLightLevelChange).get_duration(MILLISECONDS) / 250.0f;
        if (lightLevelChangeProgress > 1.0f)
            lightLevelChangeProgress = 1.0f;

        lightLevel = startLightLevel + (targetLightLevel - startLightLevel) * lightLevelChangeProgress;
    }

    // Set state
    if (activatable) {
        int index = gridPos.index(_LEVEL_WIDTH);
        if (powerGrid.is_powered(gridPos)) {
            if (toggleable && active && wasPoweredOff) {
                active          = false;
                wasPoweredOff = false;
            }
            else if (toggleable && !active && wasPoweredOff) {
                active          = true;
                wasPoweredOff = false;
            }
            else if (!toggleable && active) {
                type        = BASE_FLOOR;
                variant        = baseFloorVariant;
                toggleable  = false;
                activatable = false;
                elevated    = false;
            }
        }
        else {
            wasPoweredOff = true;
        }
    }

    // Update animations
    if (animated) {
        int microsecondsElapsed = (maintime::now() - lastFrameChange).get_duration();
        int holdTimeInMicroseconds = frameHoldTime * 1000;

        int i = 0;
        while (microsecondsElapsed > holdTimeInMicroseconds) {
            currentFrame = (currentFrame + 1) % frameCount;

            lastFrameChange        += Duration(holdTimeInMicroseconds);
            microsecondsElapsed -= holdTimeInMicroseconds;

            i++; if (i > 100) break;
        }

        //variant = spriteNames[currentFrame];
    }

    // Deal damage if tile is lava
    if (type == LAVA) {
        int index = gridPos.index(_LEVEL_WIDTH);
        damageMap[index].enemyDamageFireGround  += 5;
        damageMap[index].playerDamageFireGround += 5;

        damageMap[index].active = true;
    }
}
