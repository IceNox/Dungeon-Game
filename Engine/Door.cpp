#include "Constants.h"

#include "Door.h"
#include "Sprite.h"

Door::Door(int gX, int gY, std::string facing) : gX(gX), gY(gY), facing(facing)
{
    revealed = false;
    visible = false;

    cX = gX * cellSize + cellSize / 2;
    cY = gY * cellSize + cellSize / 2;

    if (facing == "up" || facing == "down") {
        sX = gX * cellSize;
        sY = gY * cellSize - 12;
    }
    else if (facing == "left" || facing == "right") {
        sX = gX * cellSize + 40;
        sY = gY * cellSize - 48;
    }
}

void Door::damage(DamageMap damageMap, std::vector<Command> &commands)
{
    // Spawn death particles
    Sprite particle = create_rectangle(8, 8, Color(158, 63, 0));

    if (damageMap.direction == NO_DIRECTION) damageMap.direction == UP;

    if (damageMap.direction == UP) {
        commands.push_back(Command(cX + 4, cY - 60, particle, 5, -800, -5, 800, 0.25f, 0.25f));
        commands.push_back(Command(cX + 8, cY - 52, particle, 100, -600, -100, 600, 0.25f, 0.25f));
        commands.push_back(Command(cX - 8, cY - 56, particle, -50, -600, 50, 600, 0.25f, 0.25f));
        commands.push_back(Command(cX - 8, cY - 44, particle, -100, -400, 100, 400, 0.25f, 0.25f));
    }
    else if (damageMap.direction == DOWN) {
        commands.push_back(Command(cX + 4, cY - 66, particle, 5, 800, -5, -800, 0.25f, 0.25f));
        commands.push_back(Command(cX + 8, cY - 52, particle, 100, 600, -100, -600, 0.25f, 0.25f));
        commands.push_back(Command(cX - 8, cY - 56, particle, -50, 600, 50, -600, 0.25f, 0.25f));
        commands.push_back(Command(cX - 8, cY - 44, particle, -100, 400, 100, -400, 0.25f, 0.25f));
    }
    else if (damageMap.direction == LEFT) {
        commands.push_back(Command(cX + 4, cY - 66, particle, -400, -100, 400, 100, 0.25f, 0.25f));
        commands.push_back(Command(cX + 8, cY - 52, particle, -300, -100, 300, 100, 0.25f, 0.25f));
        commands.push_back(Command(cX - 8, cY - 56, particle, -600, 100, 600, 100, 0.25f, 0.25f));
        commands.push_back(Command(cX - 8, cY - 44, particle, -500, 200, 500, 100, 0.25f, 0.25f));
    }
    else if (damageMap.direction == RIGHT) {
        commands.push_back(Command(cX + 4, cY - 66, particle, 600, -100, -600, 100, 0.25f, 0.25f));
        commands.push_back(Command(cX + 8, cY - 52, particle, 500, -100, -500, 100, 0.25f, 0.25f));
        commands.push_back(Command(cX - 8, cY - 56, particle, 400, 100, -400, 100, 0.25f, 0.25f));
        commands.push_back(Command(cX - 8, cY - 44, particle, 300, 200, -300, 100, 0.25f, 0.25f));
    }
}