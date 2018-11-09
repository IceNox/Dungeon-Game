#include "Crate.h"

#include "GlobalData.h"
#include "Constants.h"

Crate::Crate(int gX, int gY) : gX(gX), gY(gY)
{
    cX = gX * cellWidth  + cellWidth  / 2;
    cY = gY * cellHeight + cellHeight / 2;

    sX = cX + spriteOffsetX;
    sY = cY + spriteOffsetY;

    inMA            = false;
    onGround        = true;
    changedPosition = false;

    revealed = false;
}

void Crate::update_crate()
{
    if (inMA) {
        std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - MAstart;
        MAtimeElapsed = elapsed_seconds.count();
        MAprogress = MAtimeElapsed / MAlength;

        if (MAprogress > 1.0f) MAprogress = 1.0f;

        int movedBy;
        movedBy = cellSize - cellSize * (1.0f - MAprogress);

        if (MAprogress > 0.3f && changedPosition == false) {
            changedPosition = true;

            if (MAdirection == "up") {
                gY--;
            }
            else if (MAdirection == "down") {
                gY++;
            }
            else if (MAdirection == "left") {
                gX--;
            }
            else if (MAdirection == "right") {
                gX++;
            }
        }

        int height = 0;
        float xAxis = 2 * (MAprogress - 0.5f);
        float yAxis = xAxis * xAxis * -1.0f;
        height = ceil(yAxis * jumpHeight) + (int)jumpHeight + 1;

        if (MAprogress >= 1.0f) {
            inMA = false;
            onGround = true;
            changedPosition = false;
            cX = gX * cellWidth + cellWidth / 2;
            cY = gY * cellHeight + cellHeight / 2;
        }
        else {
            if (MAdirection == "up") {
                cY = sgY * cellHeight + cellHeight / 2 - movedBy;
            }
            else if (MAdirection == "down") {
                cY = sgY * cellHeight + cellHeight / 2 + movedBy;
            }
            else if (MAdirection == "left") {
                cX = sgX * cellWidth + cellWidth / 2 - movedBy;
            }
            else if (MAdirection == "right") {
                cX = sgX * cellWidth + cellWidth / 2 + movedBy;
            }
        }

        sX = cX + spriteOffsetX;
        sY = cY + spriteOffsetY - height;
    }
}

void Crate::damage(const std::vector<Tile> &tiles, std::string direction)
{
    if (!inMA) {
        if (direction == "up") {
            int ngY = gY - 1;
            if (ngY < 0) return;

            int index = ngY * levelWidth + gX;
            if (tiles[index].occupied) {
                return;
            }
            else {
                inMA        = true;
                onGround    = false;
                MAstart        = std::chrono::system_clock::now();
                MAdirection = "up";
                sgX = gX;
                sgY = gY;
            }
        }
        else if (direction == "down") {
            int ngY = gY + 1;
            if (ngY > levelHeight - 1) return;

            int index = ngY * levelWidth + gX;
            if (tiles[index].occupied) {
                return;
            }
            else {
                inMA        = true;
                onGround    = false;
                MAstart        = std::chrono::system_clock::now();
                MAdirection = "down";
                sgX = gX;
                sgY = gY;
            }
        }
        else if (direction == "left") {
            int ngX = gX - 1;
            if (ngX < 0) return;

            int index = gY * levelWidth + ngX;
            if (tiles[index].occupied) {
                return;
            }
            else {
                inMA        = true;
                onGround    = false;
                MAstart        = std::chrono::system_clock::now();
                MAdirection = "left";
                sgX = gX;
                sgY = gY;
            }
        }
        else if (direction == "right") {
            int ngX = gX + 1;
            if (ngX > levelWidth - 1) return;

            int index = gY * levelWidth + ngX;
            if (tiles[index].occupied) {
                return;
            }
            else {
                inMA        = true;
                onGround    = false;
                MAstart        = std::chrono::system_clock::now();
                MAdirection = "right";
                sgX = gX;
                sgY = gY;
            }
        }
    }
}