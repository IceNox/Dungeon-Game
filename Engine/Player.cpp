#include "Player.h"

#include "SpriteSelection.h"
#include "GlobalData.h"

#include "HealthMap.h"

#include <vector>
#include <assert.h>

Player::Player(Pos2D gPos) : gPos(gPos)
{
    health       = 20;
    armor       = 0;
    gold       = 0;
    maxHealth  = 20;

    resistancePhysical = 0.0f;
    resistanceFire       = 0.0f;
    resistanceWater    = 0.0f;
    resistanceMagic    = 0.0f;

    spriteOffset = Pos2D(-28, -88);

    hitboxRadius = 8;

    cPos = Pos2D
    (
        gPos.x * cellSize + cellSize / 2,
        gPos.y * cellSize + cellSize / 2
    );

    sPos = cPos + spriteOffset;
    
    facing = RIGHT;
    skin   = "base";

    currentAction = P_IDLE;
    spriteFacing  = RIGHT;
    spriteNumber  = 1;

    floating = false;
    onGround = true;

    dead = false;

    lastTookDamage = std::chrono::system_clock::now() - std::chrono::seconds(1);
    lastAction       = std::chrono::system_clock::now() - std::chrono::seconds(1);
}

void Player::update_player
(
    std::vector<Tile> tiles,
    std::vector<DamageMap> &damageMap,
    std::vector<Command> &commands,
    std::vector<LevelMessage> &messages,
    ScreenAnimations &screenAnimations,
    const LevelStateData &ld,
    kb::Keys &keys,
    UserData &userData,
    bool lockInput
)
{
    bool enter    = false;
    bool escape = false;
    bool up        = false;
    bool down    = false;
    bool left    = false;
    bool right    = false;
    bool use    = false;
    bool drop    = false;

    if (!lockInput) {
        get_input(keys, userData, enter, escape, up, down, left, right, use, drop);
        inventory.update(messages, cPos, facing, ld, keys, userData);
    }

    if (!inMA) {
        duration<float> elapsed_time = system_clock::now() - lastAction;
        if ((int)(elapsed_time.count() / 0.5f) % 2 == 0) {
            spriteNumber = 1;
        }
        else {
            spriteNumber = 2;
        }
    }

    if (!inMA) {
        // Check if player is on water
        if (tiles[gPos.index(_LEVEL_WIDTH)].type == WATER) {
            statusEffects.slow(0.5f, 1.0f);
            if (statusEffects.burning) statusEffects.burning = false;
        }
    }

    /*
    if (GetKeyState('1') & 0x8000) currentlySelectedItem = 0;
    if (GetKeyState('2') & 0x8000) currentlySelectedItem = 1;
    if (GetKeyState('3') & 0x8000) currentlySelectedItem = 2;
    if (GetKeyState('4') & 0x8000) currentlySelectedItem = 3;
    if (GetKeyState('5') & 0x8000) currentlySelectedItem = 4;
    if (currentlySelectedItem > items.size() - 1) currentlySelectedItem = items.size() - 1;
    */

    if        (moveBufferUp)      up    = true;
    else if (moveBufferDown)  down  = true;
    else if (moveBufferLeft)  left  = true;
    else if (moveBufferRight) right = true;

    statusEffects.update_effects();
    apply_effects(commands);

    if (statusEffects.stunned) return;

    move(keys, userData, up, down, left, right, use, tiles, damageMap, commands);
    use_items(use, damageMap, commands, screenAnimations);

    evaluate_sprite_name(this);
}

void Player::get_input(
    kb::Keys &keys,
    UserData &userData,
    bool &enter,
    bool &escape,
    bool &up,
    bool &down,
    bool &left,
    bool &right,
    bool &useItem,
    bool &dropItem
)
{
    if (keys.key_clicked(userData.keyBindings.ENTER     )) enter    = true;
    if (keys.key_clicked(userData.keyBindings.ESCAPE    )) escape   = true;
    if (keys.key_clicked(userData.keyBindings.MOVE_UP   )) up       = true;
    if (keys.key_clicked(userData.keyBindings.MOVE_DOWN )) down     = true;
    if (keys.key_clicked(userData.keyBindings.MOVE_LEFT )) left     = true;
    if (keys.key_clicked(userData.keyBindings.MOVE_RIGHT)) right    = true;
    if (keys.key_clicked(userData.keyBindings.USE_ITEM  )) useItem  = true;
    if (keys.key_clicked(userData.keyBindings.DROP_ITEM )) dropItem = true;

    /*
    if (paused) {
        if (!(GetKeyState(VK_ESCAPE) & 0x8000) && escapeIsPressed == true) {
            escapeIsPressed = false;
        }
        return;
    }

    if (GetKeyState(VK_RETURN) & 0x8000 && enterIsPressed == false) {
        enterIsPressed = true;
        enter = true;
    }
    else if (!(GetKeyState(VK_RETURN) & 0x8000) && enterIsPressed == true) {
        enterIsPressed = false;
    }

    if (GetKeyState(VK_ESCAPE) & 0x8000 && escapeIsPressed == false) {
        escapeIsPressed = true;
        escape = true;
    }
    else if (!(GetKeyState(VK_ESCAPE) & 0x8000) && escapeIsPressed == true) {
        escapeIsPressed = false;
    }

    if (GetKeyState(VK_UP) & 0x8000 && upIsPressed == false) {
        upIsPressed = true;
        up = true;
    }
    else if (!(GetKeyState(VK_UP) & 0x8000) && upIsPressed == true) {
        upIsPressed = false;
    }

    if (GetKeyState(VK_DOWN) & 0x8000 && downIsPressed == false) {
        downIsPressed = true;
        down = true;
    }
    else if (!(GetKeyState(VK_DOWN) & 0x8000) && downIsPressed == true) {
        downIsPressed = false;
    }

    if (GetKeyState(VK_LEFT) & 0x8000 && leftIsPressed == false) {
        leftIsPressed = true;
        left = true;
    }
    else if (!(GetKeyState(VK_LEFT) & 0x8000) && leftIsPressed == true) {
        leftIsPressed = false;
    }

    if (GetKeyState(VK_RIGHT) & 0x8000 && rightIsPressed == false) {
        rightIsPressed = true;
        right = true;
    }
    else if (!(GetKeyState(VK_RIGHT) & 0x8000) && rightIsPressed == true) {
        rightIsPressed = false;
    }

    if (GetKeyState(VK_SPACE) & 0x8000 && spaceIsPressed == false) {
        spaceIsPressed = true;
        space = true;
    }
    else if (!(GetKeyState(VK_SPACE) & 0x8000) && spaceIsPressed == true) {
        spaceIsPressed = false;
    }
    */
}

void Player::apply_effects
(
    std::vector<Command> &commands
)
{
    // Slow
    if (statusEffects.slowed) {
        MAlength = basePlayerMAlength / (1.0f - statusEffects.slowStrength);
    }
    else {
        MAlength = basePlayerMAlength;
    }

    // Burn
    if (statusEffects.burning && statusEffects.fireDamage) {
        damage(statusEffects.burnStrength, D_FIRE, NO_DIRECTION, commands);
        statusEffects.fireDamage = false;
    }
}

void Player::move
(
    kb::Keys &keys,
    UserData &userData,
    bool up,
    bool down,
    bool left,
    bool right,
    bool space,
    std::vector<Tile> tiles,
    std::vector<DamageMap>& damageMap,
    std::vector<Command> &commands
)
{
    if (!inMA) {
        // Evaluate input
        bool moved = false;

        Pos2D nPos = gPos;

        if (up) {
            nPos.y--;
            moved = true;
        }
        else if (down) {
            nPos.y++;
            moved = true;
        }
        else if (left) {
            nPos.x--;
            moved = true;
        }
        else if (right) {
            nPos.x++;
            moved = true;
        }

        if (keys.key_clicked(userData.keyBindings.CHANGE_DIRECTION)) {
            Direction dir = facing;

            if        (up)    facing = UP;
            else if (down)  facing = DOWN;
            else if (left)  facing = LEFT;
            else if (right) facing = RIGHT;

            if (facing == dir) return;

            //Sprite particle = baseDirectionArrowSprite;

            // Show direction arrow
            /*
            if        (facing == "right") particle.RotateClockwise(1);
            else if (facing == "down")    particle.RotateClockwise(2);
            else if (facing == "left")    particle.RotateClockwise(3);

            int pvX = 0;
            int pvY = 0;
            if        (facing == "up")    pvY = -40;
            else if (facing == "down")    pvY = 40;
            else if (facing == "left")    pvX = -40;
            else if (facing == "right") pvX = 40;

            commands.push_back(Command(cX - 12, cY - 102, particle, pvX, pvY, 0, 0, 0.25f, 0.5f));
            */

            moved = false;
        }

        moveBufferUp    = false;
        moveBufferDown    = false;
        moveBufferLeft    = false;
        moveBufferRight = false;

        /*
        for (int i = 0; i < tiles.size(); i++) {
        if (tiles[i].type == "wall" && tiles[i].x == nx && tiles[i].y == ny) {
        moved = false;
        break;
        }
        }
        */

        if (tiles[nPos.index(_LEVEL_WIDTH)].occupied) {
            moved = false;
        }

        // If a valid movement command was issued, set up movement animation variables
        if (moved == true) {
            currentAction = P_MOVING;
            spriteNumber  = 1;

            inMA     = true;
            onGround = false;
            sgPos     = gPos;

            c_MAlength = MAlength;

            lastAction = std::chrono::system_clock::now();
            MAstart    = std::chrono::system_clock::now();

            if (up) {
                MAdirection = UP;
            }
            else if (down) {
                MAdirection = DOWN;
            }
            else if (left) {
                MAdirection = LEFT;
            }
            else if (right) {
                MAdirection = RIGHT;
            }

            facing = MAdirection;

            // Set the tile the player is moving to as occupied to prevent other AI from entering it
            if        (facing == UP)        tiles[gPos.index(_LEVEL_WIDTH, UP, 1)    ].occupied = true;
            else if (facing == DOWN)    tiles[gPos.index(_LEVEL_WIDTH, DOWN, 1)    ].occupied = true;
            else if (facing == LEFT)    tiles[gPos.index(_LEVEL_WIDTH, LEFT, 1)    ].occupied = true;
            else if (facing == RIGHT)    tiles[gPos.index(_LEVEL_WIDTH, RIGHT, 1)].occupied = true;
        }
        if        (facing == LEFT)  spriteFacing = LEFT;
        else if (facing == RIGHT) spriteFacing = RIGHT;
    }
    else {
        // Update time
        std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - MAstart;

        MAtimeElapsed = elapsed_seconds.count();
        MAprogress      = MAtimeElapsed / c_MAlength;

        // Calculate distance moved while in the MA
        int movedBy;
        movedBy = cellSize - cellSize * (1.0f - MAprogress) * (1.0f - MAprogress);

        // If the player moved halfway to another tile, change his coordinates and update tile lighting info
        if (MAprogress > 0.3f && changedPosition == false) {
            changedPosition = true;

            /*
            if        (MAdirection == UP) {
                gPos.y--;
            }
            else if (MAdirection == DOWN) {
                gPos.y++;
            }
            else if (MAdirection == LEFT) {
                gPos.x--;
            }
            else if (MAdirection == RIGHT) {
                gPos.x++;
            }
            */

            gPos.move(MAdirection, 1);
        }

        if (MAprogress > 0.5f) {
            spriteNumber = 2;
        }

        // Calculate current height
        int height = 10;
        if (!floating) {
            float xAxis = 2 * (MAprogress - 0.5f);
            float yAxis = xAxis * xAxis * -1.0f;

            height = ceil(yAxis * jumpHeight) + (int)jumpHeight + 1;
        }

        // End MA if the progress is 100%
        if (MAprogress >= 1.0f) {
            end_MA();
        }
        // Otherwise, update player position
        else {
            if        (MAdirection == UP) {
                cPos.y = sgPos.y * cellHeight + cellHeight / 2 - movedBy;
            }
            else if (MAdirection == DOWN) {
                cPos.y = sgPos.y * cellHeight + cellHeight / 2 + movedBy;
            }
            else if (MAdirection == LEFT) {
                cPos.x = sgPos.x * cellWidth + cellWidth / 2 - movedBy;
            }
            else if (MAdirection == RIGHT) {
                cPos.x = sgPos.x * cellWidth + cellWidth / 2 + movedBy;
            }
        }

        // Update player sprite position
        sPos = cPos + spriteOffset;

        if (inMA) sPos.y -= height;

        // Movement buffering in the middle of the animation
        if (MAprogress > 0.8f) {
            if        (up)    moveBufferUp    = true;
            else if (down)  moveBufferDown  = true;
            else if (left)  moveBufferLeft  = true;
            else if (right) moveBufferRight = true;
        }
    }
}

void Player::end_MA()
{
    inMA            = false;
    onGround        = true;
    changedPosition = false;
    currentAction    = P_IDLE;
    spriteNumber    = 1;

    //cPos.x = gPos.x * cellSize + cellSize / 2;
    //cPos.y = gPos.y * cellSize + cellSize / 2;

    cPos = gPos * cellSize + Pos2D(cellSize / 2);

    sPos = cPos + spriteOffset;
}

void Player::use_items
(
    bool useItem,
    std::vector<DamageMap>& damageMap,
    std::vector<Command> &commands,
    ScreenAnimations &screenAnimations
)
{
    if (!inMA) {
        if (useItem) {
            //items[currentlySelectedItem].use_item(damageMap, commands, screenAnimations, gPos, facing);
        }
    }
}

void Player::heal(int amount)
{
    if (health + amount > maxHealth) health = maxHealth;
    else health += amount;
}

void Player::damage
(
    int amount,
    DamageType type,
    Direction direction,
    std::vector<Command> &commands
)
{
    DamageMap dm;
    dm.direction    = direction;
    
    if        (type == D_PHYSICAL)    dm.playerDamagePhysical    += amount;
    else if (type == D_FIRE)        dm.playerDamageFire        += amount;
    else if (type == D_WATER)        dm.playerDamageWater    += amount;

    damage(dm, commands);
}

void Player::damage
(
    DamageMap damageMap,
    std::vector<Command> &commands
)
{
    using namespace std::chrono;

    time_point<system_clock> now = system_clock::now();
    duration<float>    elapsed_time = now - lastTookDamage;

    if (elapsed_time < microseconds((int)(damageCooldown * 1000000))) return;

    int amount = 0;

    // Count total damage
    int amountP = damageMap.playerDamagePhysical;
    int amountF = damageMap.playerDamageFire;
    int amountW = damageMap.playerDamageWater;
    int amountM = damageMap.playerDamageMagic;

    if (onGround) {
        amountP += damageMap.playerDamagePhysicalGround;
        amountF += damageMap.playerDamageFireGround;
        amountW += damageMap.playerDamageWaterGround;
        amountM += damageMap.playerDamageMagicGround;
    }

    if (amountP > 0) {
        amountP -= ceil(resistancePhysical);
        if (amountP < 1) amountP = 1;
    }
    if (amountF > 0) {
        amountF -= ceil(resistanceFire);
        if (amountF < 1) amountF = 1;
    }
    if (amountW > 0) {
        amountW -= ceil(resistanceWater);
        if (amountW < 1) amountW = 1;
    }
    if (amountM > 0) {
        amountM -= ceil(resistanceMagic);
        if (amountM < 1) amountM = 1;
    }

    amount += amountP + amountF + amountW + amountM;

    if (amount == 0) return;

    tookDamage = true;
    damageAmount += amount;
    
    // Apply efects
    if (amountF > 0) statusEffects.burn(amountF / 5 + 1, 3);

    // Deal damage
    lastTookDamage = system_clock::now();

    if (armor > 0) {
        if (armor - amount < 0) {
            amount -= armor;
            armor   = 0;
            health -= amount;
            return;
        }
        else {
            armor -= amount;
            return;
        }
    }

    health -= amount;
    if (health < 0) {
        health = 0;
    }
    if (health == 0) {
        dead = true;
    }
}
