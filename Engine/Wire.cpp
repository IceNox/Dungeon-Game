#include "Wire.h"

void Wire::set_wire(Pos2D gPos)
{
    this->gPos = gPos;

    gate = false;
    gateType = "null";

    red = false;
    blue = false;
    green = false;
    yellow = false;

    redPowered = false;
    bluePowered = false;
    greenPowered = false;
    yellowPowered = false;

    leftPowered = false;
    rightPowered = false;
    facing = NO_DIRECTION;
}

void Wire::power(std::vector<Wire> &wires, std::string side, std::string color, int w, int h)
{
    /*
    int up   = (gY - 1) * w + gX;
    int down = (gY + 1) * w + gX;
    int left  = gY * w + gX - 1;
    int right = gY * w + gX + 1;

    if (!gate) {
        if        (color == "red" && red && !redPowered) {
            redPowered = true;
        }
        else if (color == "blue" && blue && !bluePowered) {
            bluePowered = true;
        }
        else if (color == "green" && green && !greenPowered) {
            greenPowered = true;
        }
        else if (color == "yellow" && yellow && !yellowPowered) {
            yellowPowered = true;
        }

        if        (color == "red") {
            if (wires[up].red && !wires[up].redPowered) {
                wires[up].power(wires, "down", "red", w, h);
            }
            if (wires[down].red && !wires[down].redPowered) {
                wires[down].power(wires, "up", "red", w, h);
            }
            if (wires[left].red && !wires[left].redPowered) {
                wires[left].power(wires, "right", "red", w, h);
            }
            if (wires[right].red && !wires[right].redPowered) {
                wires[right].power(wires, "left", "red", w, h);
            }

            if (wires[up].gate) wires[up].power(wires, "down", "red", w, h);
            if (wires[down].gate) wires[down].power(wires, "up", "red", w, h);
            if (wires[left].gate) wires[left].power(wires, "right", "red", w, h);
            if (wires[right].gate) wires[right].power(wires, "left", "red", w, h);
        }
        else if (color == "blue") {
            if (wires[up].blue && !wires[up].bluePowered) {
                wires[up].power(wires, "down", "blue", w, h);
            }
            if (wires[down].blue && !wires[down].bluePowered) {
                wires[down].power(wires, "up", "blue", w, h);
            }
            if (wires[left].blue && !wires[left].bluePowered) {
                wires[left].power(wires, "right", "blue", w, h);
            }
            if (wires[right].blue && !wires[right].bluePowered) {
                wires[right].power(wires, "left", "blue", w, h);
            }

            if (wires[up].gate) wires[up].power(wires, "down", "blue", w, h);
            if (wires[down].gate) wires[down].power(wires, "up", "blue", w, h);
            if (wires[left].gate) wires[left].power(wires, "right", "blue", w, h);
            if (wires[right].gate) wires[right].power(wires, "left", "blue", w, h);
        }
        else if (color == "green") {
            if (wires[up].green && !wires[up].greenPowered) {
                wires[up].power(wires, "down", "green", w, h);
            }
            if (wires[down].green && !wires[down].greenPowered) {
                wires[down].power(wires, "up", "green", w, h);
            }
            if (wires[left].green && !wires[left].greenPowered) {
                wires[left].power(wires, "right", "green", w, h);
            }
            if (wires[right].green && !wires[right].greenPowered) {
                wires[right].power(wires, "left", "green", w, h);
            }

            if (wires[up].gate) wires[up].power(wires, "down", "green", w, h);
            if (wires[down].gate) wires[down].power(wires, "up", "green", w, h);
            if (wires[left].gate) wires[left].power(wires, "right", "green", w, h);
            if (wires[right].gate) wires[right].power(wires, "left", "green", w, h);
        }
        else if (color == "yellow") {
            if (wires[up].yellow && !wires[up].yellowPowered) {
                wires[up].power(wires, "down", "yellow", w, h);
            }
            if (wires[down].yellow && !wires[down].yellowPowered) {
                wires[down].power(wires, "up", "yellow", w, h);
            }
            if (wires[left].yellow && !wires[left].yellowPowered) {
                wires[left].power(wires, "right", "yellow", w, h);
            }
            if (wires[right].yellow && !wires[right].yellowPowered) {
                wires[right].power(wires, "left", "yellow", w, h);
            }

            if (wires[up].gate) wires[up].power(wires, "down", "yellow", w, h);
            if (wires[down].gate) wires[down].power(wires, "up", "yellow", w, h);
            if (wires[left].gate) wires[left].power(wires, "right", "yellow", w, h);
            if (wires[right].gate) wires[right].power(wires, "left", "yellow", w, h);
        }
    }
    else {
        if (gateType == "and") {
            if        (facing == "up") {
                if        (side == "left" && !leftPowered) {
                    leftPowered = true;
                }
                else if (side == "right" && !rightPowered) {
                    rightPowered = true;
                }
                else {
                    return;
                }

                if (leftPowered && rightPowered) {
                    if (wires[up].red) wires[up].power(wires, "down", "red", w, h);
                    if (wires[up].blue) wires[up].power(wires, "down", "blue", w, h);
                    if (wires[up].green) wires[up].power(wires, "down", "green", w, h);
                    if (wires[up].yellow) wires[up].power(wires, "down", "yellow", w, h);
                }
            }
            else if (facing == "down") {
                if (side == "left" && !rightPowered) {
                    rightPowered = true;
                }
                else if (side == "right" && !leftPowered) {
                    leftPowered = true;
                }
                else {
                    return;
                }

                if (leftPowered && rightPowered) {
                    if (wires[down].red) wires[down].power(wires, "up", "red", w, h);
                    if (wires[down].blue) wires[down].power(wires, "up", "blue", w, h);
                    if (wires[down].green) wires[down].power(wires, "up", "green", w, h);
                    if (wires[down].yellow) wires[down].power(wires, "up", "yellow", w, h);
                }
            }
            else if (facing == "left") {
                if (side == "down" && !leftPowered) {
                    leftPowered = true;
                }
                else if (side == "up" && !rightPowered) {
                    rightPowered = true;
                }
                else {
                    return;
                }

                if (leftPowered && rightPowered) {
                    if (wires[left].red) wires[left].power(wires, "right", "red", w, h);
                    if (wires[left].blue) wires[left].power(wires, "right", "blue", w, h);
                    if (wires[left].green) wires[left].power(wires, "right", "green", w, h);
                    if (wires[left].yellow) wires[left].power(wires, "right", "yellow", w, h);
                }
            }
            else if (facing == "right") {
                if (side == "up" && !leftPowered) {
                    leftPowered = true;
                }
                else if (side == "down" && !rightPowered) {
                    rightPowered = true;
                }
                else {
                    return;
                }

                if (leftPowered && rightPowered) {
                    if (wires[right].red) wires[right].power(wires, "left", "red", w, h);
                    if (wires[right].blue) wires[right].power(wires, "left", "blue", w, h);
                    if (wires[right].green) wires[right].power(wires, "left", "green", w, h);
                    if (wires[right].yellow) wires[right].power(wires, "left", "yellow", w, h);
                }
            }
        }
        else if (gateType == "or") {
            if        (facing == "up") {
                if (side == "left" && !leftPowered) {
                    leftPowered = true;
                }
                else if (side == "right" && !rightPowered) {
                    rightPowered = true;
                }
                else {
                    return;
                }

                if (leftPowered || rightPowered) {
                    if (wires[up].red) wires[up].power(wires, "down", "red", w, h);
                    if (wires[up].blue) wires[up].power(wires, "down", "blue", w, h);
                    if (wires[up].green) wires[up].power(wires, "down", "green", w, h);
                    if (wires[up].yellow) wires[up].power(wires, "down", "yellow", w, h);
                }
            }
            else if (facing == "down") {
                if (side == "left" && !rightPowered) {
                    rightPowered = true;
                }
                else if (side == "right" && !leftPowered) {
                    leftPowered = true;
                }
                else {
                    return;
                }

                if (leftPowered || rightPowered) {
                    if (wires[down].red) wires[down].power(wires, "up", "red", w, h);
                    if (wires[down].blue) wires[down].power(wires, "up", "blue", w, h);
                    if (wires[down].green) wires[down].power(wires, "up", "green", w, h);
                    if (wires[down].yellow) wires[down].power(wires, "up", "yellow", w, h);
                }
            }
            else if (facing == "left") {
                if (side == "down" && !leftPowered) {
                    leftPowered = true;
                }
                else if (side == "up" && !rightPowered) {
                    rightPowered = true;
                }
                else {
                    return;
                }

                if (leftPowered || rightPowered) {
                    if (wires[left].red) wires[left].power(wires, "right", "red", w, h);
                    if (wires[left].blue) wires[left].power(wires, "right", "blue", w, h);
                    if (wires[left].green) wires[left].power(wires, "right", "green", w, h);
                    if (wires[left].yellow) wires[left].power(wires, "right", "yellow", w, h);
                }
            }
            else if (facing == "right") {
                if (side == "up" && !leftPowered) {
                    leftPowered = true;
                }
                else if (side == "down" && !rightPowered) {
                    rightPowered = true;
                }
                else {
                    return;
                }

                if (leftPowered || rightPowered) {
                    if (wires[right].red) wires[right].power(wires, "left", "red", w, h);
                    if (wires[right].blue) wires[right].power(wires, "left", "blue", w, h);
                    if (wires[right].green) wires[right].power(wires, "left", "green", w, h);
                    if (wires[right].yellow) wires[right].power(wires, "left", "yellow", w, h);
                }
            }
        }
    }
    */
}