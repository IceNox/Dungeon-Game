#pragma once

#include "Constants.h"
#include "PowerGrid.h"

#include <string>
#include <vector>

class Lamp
{
public:
    int gX;
    int gY;

    int sX;
    int sY;

    std::string type;

    bool powered;
    bool toggleable;
    bool wasPoweredOff;
    bool wasPoweredOn;

    bool revealed;
    bool visible;

    Lamp(int gX, int gY, std::string type, bool toggleable);
    //void update_lamp(std::vector<Wire> &wires);
};
