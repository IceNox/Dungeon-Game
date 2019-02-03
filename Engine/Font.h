#pragma once

#include "Sprite.h"

#include <string>
#include <vector>

class Font
{
public:
    std::string name;
    std::vector<Sprite> character;
    std::vector<int> characterW;
    std::vector<int> characterH;
    int kerning;

    Font() {};
};
