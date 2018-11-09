#pragma once

#include "Position.h"

#include <string>
#include <vector>

class Wire
{
public:
    Pos2D gPos;

    bool gate;
    std::string gateType;

    bool red;
    bool blue;
    bool green;
    bool yellow;

    bool redPowered;
    bool bluePowered;
    bool greenPowered;
    bool yellowPowered;

    bool leftPowered;
    bool rightPowered;
    Direction facing;

    Wire() {};
    void set_wire(Pos2D gPos);
    void power(std::vector<Wire> &wires, std::string side, std::string color, int w, int h);
};