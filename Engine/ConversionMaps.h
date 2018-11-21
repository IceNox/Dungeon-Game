#pragma once

#include "Constants.h"

#include <unordered_map>

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
