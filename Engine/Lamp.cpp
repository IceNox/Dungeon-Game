#include "Lamp.h"

#include "GlobalData.h"

Lamp::Lamp(int gX, int gY, std::string type, bool toggleable) : gX(gX), gY(gY), type(type), toggleable(toggleable)
{
    revealed = false;

    powered = false;
    wasPoweredOff = false;

    sX = gX * cellSize + 36;
    sY = gY * cellSize + 28;
}

void Lamp::update_lamp(std::vector<Wire> &wires)
{
    if (
        wires[gY * levelWidth + gX].redPowered ||
        wires[gY * levelWidth + gX].bluePowered ||
        wires[gY * levelWidth + gX].greenPowered ||
        wires[gY * levelWidth + gX].yellowPowered
        ) 
    {
        if        (toggleable && !powered && wasPoweredOff) {
            powered = true;
            wasPoweredOff = false;
        }
        else if (toggleable && powered && wasPoweredOff) {
            powered = false;
            wasPoweredOff = false;
        }
        else if (!toggleable && !powered) {
            powered = true;
        }
    }
    else {
        if (!toggleable && powered) {
            powered = false;
        }

        wasPoweredOff = true;
    }
}