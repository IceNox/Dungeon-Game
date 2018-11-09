#pragma once

#include "Sprite.h"

#include <string>
#include <vector>

class Font
{
public:
    std::vector<Sprite> character;
    std::vector<int>    characterW;

    Font();

    void set_variables(std::string type);
};