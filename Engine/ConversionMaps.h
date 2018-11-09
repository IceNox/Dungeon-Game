#pragma once

#include "Constants.h"

#include "Position.h"

#include <unordered_map>

// String -> Direction map
std::unordered_map<std::string, Direction> directionMap =
{
    { "up"      , UP },
    { "right" , RIGHT },
    { "down"  , DOWN },
    { "left"  , LEFT },
    { "none"  , NO_DIRECTION },
    { "u"      , UP },
    { "r"      , RIGHT },
    { "d"      , DOWN },
    { "l"      , LEFT },
    { "n"      , NO_DIRECTION }
};

// Direction -> Vector map
std::unordered_map<Direction, Pos2D> directionVectorMap =
{
    { LEFT    ,{ -1, 0 } },
    { UP    ,{ 0, -1 } },
    { RIGHT ,{ 1, 0 } },
    { DOWN    ,{ 0, 1 } },
    { NO_DIRECTION ,{ 0, 0 } }
};

// String -> DamageType map
std::unordered_map<std::string, DamageType> damageTypeMap =
{
    { "physical", D_PHYSICAL },
    { "air"        , D_AIR },
    { "earth"    , D_EARTH },
    { "fire"    , D_FIRE },
    { "water"    , D_WATER },
    { "holy"    , D_HOLY },
    { "demonic"    , D_DEMONIC },
    { "p"        , D_PHYSICAL },
    { "a"        , D_AIR },
    { "e"        , D_EARTH },
    { "f"        , D_FIRE },
    { "w"        , D_WATER },
    { "h"        , D_HOLY },
    { "d"        , D_DEMONIC }
};