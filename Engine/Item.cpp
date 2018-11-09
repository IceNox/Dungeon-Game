#include "Item.h"

#include "GlobalData.h"
#include "Constants.h"

using namespace std::chrono;

Item::Item(std::string itemName)
{
    if (itemName == "wooden_sword") {
        name       = "sword";
        type       = "wooden";
        spriteName = "wooden_sword";

        cooldown    = 500;
        activatable = true;
    }
    else if (itemName == "steel_sword") {
        name       = "sword";
        type       = "steel";
        spriteName = "steel_sword";

        cooldown = 500;
        activatable = true;
    }

    lastUsed = system_clock::now() - milliseconds(cooldown);
}

void Item::use_item
(
    std::vector<DamageMap>& damageMap,
    std::vector<Command> &commands,
    ScreenAnimations &screenAnimations,
    Pos2D gPos,
    Direction facing
)
{
    /*
    if (!activatable) return;

    std::chrono::duration<float> timeElapsed = system_clock::now() - lastUsed;
    if (timeElapsed < milliseconds(cooldown)) return;

    if (name == "sword") {
        int index = gPos.index(_LEVEL_WIDTH);

        if        (facing == UP     && gPos.y > 0                  ) index -= _LEVEL_WIDTH;
        else if (facing == DOWN  && gPos.y < _LEVEL_HEIGHT - 1) index += _LEVEL_WIDTH;
        else if (facing == LEFT  && gPos.x > 0                  ) index -= 1;
        else if (facing == RIGHT && gPos.x < _LEVEL_HEIGHT - 1) index += 1;

        if (type == "wooden") damageMap[index].enemyDamagePhysical = 2;
        if (type == "steel")  damageMap[index].enemyDamagePhysical = 4;

        if        (facing == UP   ) damageMap[index].direction = UP;
        else if (facing == DOWN ) damageMap[index].direction = DOWN;
        else if (facing == LEFT ) damageMap[index].direction = LEFT;
        else if (facing == RIGHT) damageMap[index].direction = RIGHT;

        damageMap[index].active        = true;
        damageMap[index].startShake = true;

        int soX;
        int soY;

        int sprX;
        int sprY = 72;

        if        (facing == "up") {
            soX = 0;
            soY = -132;

            sprX = 8;
        }
        else if (facing == "down") {
            soX = -8;
            soY = -36;

            sprX = 8;
        }
        else if (facing == "left") {
            soX = -68;
            soY = -72;

            sprX = 24;
        }
        else if (facing == "right") {
            soX = 44;
            soY = -72;

            sprX = 24;
        }
        
        //visualEffects.push_back(VisualEffect(x * cellSize + cellSize / 2 + soX, y * cellSize + cellSize / 2 + soY, false, "attack_chop_" + facing + "_effect", sprX, sprY, 0.01f, 7));
        commands.push_back(Command(x * cellSize + cellSize / 2 + soX, y * cellSize + cellSize / 2 + soY, false, "attack_chop_" + facing + "_effect", sprX, sprY, 0.01f, 7, 1.0f));
        
    }

    lastUsed = std::chrono::system_clock::now();
    */
}