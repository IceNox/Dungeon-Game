#include "Lich.h"

#include "GlobalData.h"
#include "Constants.h"
#include "Functions.h"

#include <cstdlib>

Lich::Lich(
    int            gX,
    int            gY,
    std::string type,
    std::vector<Command> &commands,
    bool        spawn
)
{
    using namespace std::chrono;

    this->gX = gX;
    this->gY = gY;
    this->type = type;

    facing = "left";

    currentAction = "idle";
    spriteFacing  = "left";
    spriteNumber  = "1";

    cX = gX * cellSize + cellSize / 2;
    cY = gY * cellSize + cellSize / 2;

    floating = true;
    onGround = false;

    if (type == "small") {
        health            = 6;
        armor            = 0;
        maxHealth        = 6;
        boltDamage        = 5;
        healthbarHeight = 110;
        spriteOffsetX    = -40;
        spriteOffsetY    = -120;
        hitboxRadius    = 20;
        gold            = 40;

        resistancePhysical = 0.0f;
        resistanceFire       = 0.0f;
        resistanceWater    = 0.0f;
        resistanceMagic    = 0.0f;

        immunePhysical = false;
        immuneFire       = false;
        immuneWater       = false;
        immuneMagic       = true;

        deathParticleColor = Color(100, 100, 100);
    }

    sX = cX + spriteOffsetX;
    sY = cY + spriteOffsetY;

    lastAction       = system_clock::now();
    lastTookDamage = system_clock::now();

    if (spawn) {
        gold = 0;

        spawning = true;
        spawnLength = 1.0f;

        //visualEffects.push_back(VisualEffect());
        commands.push_back(Command(0, 0, false, "temp", 0, 0, 0.1f, 10, 1.0f));

        spawnStart = system_clock::now();
    }
}

void Lich::update_lich
(
    Player &player,
    std::vector<Tile> &tiles,
    std::vector<DamageMap> &damageMap,
    std::vector<Command> &commands
)
{
    if (spawning) {
        spawn();
        return;
    }
    if (!active) {
        return;
    }

    cast_bolt(player, tiles, commands);
    spawn_zombie(player, tiles, commands);
    move(player, tiles);
}

void Lich::spawn()
{
    duration<float> elapsed_time = system_clock::now() - spawnStart;

    float secondsElapsed = elapsed_time.count();
    float progress = secondsElapsed / spawnLength;

    if (progress >= 0.95f) {
        spawning = false;

        lastAction = system_clock::now() - milliseconds(500);
    }
}

void Lich::cast_bolt
(
    Player &player,
    std::vector<Tile> &tiles,
    std::vector<Command> &commands
)
{
    using namespace std::chrono;

    if (!inMA && !inBA && !inSA) {
        duration<float> elapsed_time;

        elapsed_time = system_clock::now() - lastAction;
        if (elapsed_time < microseconds((int)(actionCooldown * 1000000))) return;

        elapsed_time = system_clock::now() - BAlastCast;
        if (elapsed_time < microseconds((int)(boltCooldown * 1000000))) return;

        if (distance(Pos2D(player.gX, player.gY), Pos2D(gX, gY)) > 10) return;

        if (player.gX == gX) {
            if (player.gY < gY) {
                int x = gX;
                int y1 = player.gY + 1;
                int y2 = gY - 1;

                for (int i = y1; i < y2; i++) {
                    if (tiles[i * levelWidth + x].type == WALL && tiles[i * levelWidth + x].active) {
                        return;
                    }
                }

                inBA        = true;
                BAdirection = "up";
                BAstart        = system_clock::now();

                facing          = "up";
                currentAction = "casting_bolt";
                spriteNumber  = "1";

                actionCooldown = 1.0f;
                lastAction = system_clock::now();
            }
            else if (player.gY > gY) {
                int x  = gX;
                int y1 = gY + 1;
                int y2 = player.gY - 1;

                for (int i = y1; i < y2; i++) {
                    if (tiles[i * levelWidth + x].type == WALL && tiles[i * levelWidth + x].active) {
                        return;
                    }
                }

                inBA        = true;
                BAdirection = "down";
                BAstart        = system_clock::now();

                facing          = "down";
                currentAction = "casting_bolt";
                spriteNumber  = "1";

                actionCooldown = 1.0f;
                lastAction       = system_clock::now();
            }
        }
        else if (player.gY == gY) {
            if (player.gX < gX) {
                int x1 = player.gX + 1;
                int x2 = gX - 1;
                int y = gY;

                for (int i = x1; i < x2; i++) {
                    if (tiles[y * levelWidth + i].type == WALL && tiles[y * levelWidth + i].active) {
                        return;
                    }
                }

                inBA        = true;
                BAdirection = "left";
                BAstart        = system_clock::now();

                facing          = "left";
                spriteFacing  = "left";
                currentAction = "casting_bolt";
                spriteNumber  = "1";

                actionCooldown = 1.0f;
                lastAction       = system_clock::now();
            }
            else if (player.gX > gX) {
                int x1 = gX + 1;
                int x2 = player.gX - 1;
                int y  = gY;

                for (int i = x1; i < x2; i++) {
                    if (tiles[y * levelWidth + i].type == WALL && tiles[y * levelWidth + i].active) {
                        return;
                    }
                }

                inBA        = true;
                BAdirection = "right";
                BAstart        = system_clock::now();

                facing          = "right";
                spriteFacing  = "right";
                currentAction = "casting_bolt";
                spriteNumber  = "1";

                actionCooldown = 1.0f;
                lastAction       = system_clock::now();
            }
        }
    }
    else if (inBA) {
        duration<float> elapsed_seconds = system_clock::now() - BAstart;

        BAtimeElapsed = elapsed_seconds.count();
        BAprogress = BAtimeElapsed / BAlength;

        if (BAprogress < 0.3f) {
            spriteNumber = "1";
        }
        else if (BAprogress < 1.0f) {
            if (spriteNumber == "1") {
                std::string spriteName = "lich_bolt_" + BAdirection;

                //if        (BAdirection == "up")    entities.push_back(Entity(gX, gY, cellSize / 2, cellSize / 2 - 40, boltDamage, 0, 20, -20, -20 - 30, true, false, 0.0f, -500.0f, 10000, 10000, true, spriteName, 40, 40, 2, 0.2f));
                //else if (BAdirection == "down")  entities.push_back(Entity(gX, gY, cellSize / 2, cellSize / 2 + 40, boltDamage, 0, 20, -20, -20 - 30, true, false, 0.0f,  500.0f, 10000, 10000, true, spriteName, 40, 40, 2, 0.2f));
                //else if (BAdirection == "left")  entities.push_back(Entity(gX, gY, cellSize / 2 - 40, cellSize / 2, boltDamage, 0, 20, -20, -20 - 30, true, false, -500.0f, 0.0f, 10000, 10000, true, spriteName, 40, 40, 2, 0.2f));
                //else if (BAdirection == "right") entities.push_back(Entity(gX, gY, cellSize / 2 + 40, cellSize / 2, boltDamage, 0, 20, -20, -20 - 30, true, false,  500.0f, 0.0f, 10000, 10000, true, spriteName, 40, 40, 2, 0.2f));

                if        (BAdirection == "up")    commands.push_back(Command(gX, gY, cellSize / 2, cellSize / 2 - 40, boltDamage, 0, 20, -20, -20 - 30, true, false, 0.0f, -500.0f, 10000, 10000, true, spriteName, 40, 40, 2, 0.2f));
                else if (BAdirection == "down")  commands.push_back(Command(gX, gY, cellSize / 2, cellSize / 2 + 40, boltDamage, 0, 20, -20, -20 - 30, true, false, 0.0f, 500.0f, 10000, 10000, true, spriteName, 40, 40, 2, 0.2f));
                else if (BAdirection == "left")  commands.push_back(Command(gX, gY, cellSize / 2 - 40, cellSize / 2, boltDamage, 0, 20, -20, -20 - 30, true, false, -500.0f, 0.0f, 10000, 10000, true, spriteName, 40, 40, 2, 0.2f));
                else if (BAdirection == "right") commands.push_back(Command(gX, gY, cellSize / 2 + 40, cellSize / 2, boltDamage, 0, 20, -20, -20 - 30, true, false, 500.0f, 0.0f, 10000, 10000, true, spriteName, 40, 40, 2, 0.2f));
            }

            spriteNumber = "2";
        }
        else if (BAprogress >= 1.0f) {
            inBA          = false;
            currentAction = "idle";
            spriteNumber  = "1";
            lastAction    = system_clock::now();
            BAlastCast    = system_clock::now();
        }
    }
}

void Lich::spawn_zombie
(
    Player &player,
    std::vector<Tile> &tiles,
    std::vector<Command> &commands
)
{
    using namespace std::chrono;

    if (!inMA && !inBA && !inSA) {
        duration<float> elapsed_time;

        elapsed_time = system_clock::now() - lastAction;
        if (elapsed_time < microseconds((int)(actionCooldown * 1000000))) return;

        elapsed_time = system_clock::now() - SAlastCast;
        if (elapsed_time < microseconds((int)(spawnCooldown * 1000000))) return;

        if (distance(Pos2D(player.gX, player.gY), Pos2D(gX, gY)) > 7) return;

        inSA    = true;
        SAstart = system_clock::now();

        currentAction = "spawning_zombie";
        spriteNumber  = "1";

        actionCooldown = 1.0f;
        lastAction       = system_clock::now();

        srand(gX * gY * player.gX * player.gY + player.health + player.armor + player.cX * player.cY * player.currentlySelectedItem);

        for (int i = 0; i < 100; i++) {
            int x = player.gX - 2 + (rand() % 5);
            int y = player.gY - 2 + (rand() % 5);

            int index = y * levelWidth + x;

            if (i == 99) {
                currentAction = "idle";
                spriteNumber  = "1";
                inSA          = false;
                SAlastCast      = system_clock::now();
                break;
            }

            if (distance(Pos2D(x, y), Pos2D(player.gX, player.gY)) > 10) continue;
            if (tiles[index].occupied) continue;
            if (x < 0 || x >= levelWidth || y < 0 || y >= levelHeight) continue;

            //zombies.push_back(Zombie(x, y, type, visualEffects, true));
            commands.push_back(Command(x, y, "zombie", type, true));

            break;
        }
    }
    else if (inSA) {
        duration<float> elapsed_seconds = system_clock::now() - SAstart;

        SAtimeElapsed = elapsed_seconds.count();
        SAprogress      = SAtimeElapsed / SAlength;

        if (SAprogress >= 1.0f) {
            inSA          = false;
            currentAction = "idle";
            spriteNumber  = "1";
            SAlastCast      = system_clock::now();
        }
    }
}

void Lich::move
(
    Player &player,
    std::vector<Tile> &tiles
)
{
    using namespace std::chrono;

    if (!inMA && !inAA) {
        if (distance(Pos2D(gX, gY), Pos2D(player.gX, player.gY)) < 5) return;

        duration<float> elapsed_time = system_clock::now() - lastAction;
        if (elapsed_time < microseconds((int)(actionCooldown * 1000000))) return;

        std::string direction;

        if (player.gY == gY) {
            if (player.gX < gX) direction = "left";
            else                direction = "right";
        }
        else if (player.gX == gX) {
            if (player.gY < gY) direction = "up";
            else                direction = "down";
        }
        else {
            if (facing == "up") {
                if (player.gY < gY) direction = "up";
                else {
                    if (player.gX < gX) direction = "left";
                    else                direction = "right";
                }
            }
            else if (facing == "down") {
                if (player.gY > gY) direction = "down";
                else {
                    if (player.gX < gX) direction = "left";
                    else                direction = "right";
                }
            }
            else if (facing == "left") {
                if (player.gX < gX) direction = "left";
                else                direction = "right";
            }
            else if (facing == "right") {
                if (player.gX < gX) direction = "left";
                else                direction = "right";
            }
        }

        if (direction == "up") {
            if (tiles[(gY - 1) * levelWidth + gX].occupied) {
                if (player.gX < gX) direction = "left";
                else if (player.gX > gX) direction = "right";
                else                     direction = "up";
            }
        }
        else if (direction == "down") {
            if (tiles[(gY + 1) * levelWidth + gX].occupied) {
                if (player.gX < gX) direction = "left";
                else if (player.gX > gX) direction = "right";
                else                     direction = "down";
            }
        }
        else if (direction == "left") {
            if (tiles[gY * levelWidth + gX - 1].occupied) {
                if (player.gY < gY) direction = "up";
                else if (player.gY > gY) direction = "down";
                else                     direction = "left";
            }
        }
        else if (direction == "right") {
            if (tiles[gY * levelWidth + gX + 1].occupied) {
                if (player.gY < gY) direction = "up";
                else if (player.gY > gY) direction = "down";
                else                     direction = "right";
            }
        }

        if (direction == "up"    && tiles[(gY - 1) * levelWidth + gX].occupied) return;
        if (direction == "down"  && tiles[(gY + 1) * levelWidth + gX].occupied) return;
        if (direction == "left"     && tiles[gY * levelWidth + gX - 1].occupied) return;
        if (direction == "right" && tiles[gY * levelWidth + gX + 1].occupied) return;

        currentAction = "idle";
        spriteNumber  = "1";

        inMA        = true;
        MAdirection = direction;
        MAstart        = system_clock::now();
        sgX = gX;
        sgY = gY;

        c_MAlength = MAlength;

        facing = direction;
        if        (facing == "left")  spriteFacing = "left";
        else if (facing == "right") spriteFacing = "right";

        actionCooldown = 1.5f;
        lastAction = system_clock::now();

        // Set the tile the barbarian is moving to as occupied to prevent other AI / player from entering it
        if        (direction == "up")    tiles[(gY - 1) * levelWidth + gX].occupied = true;
        else if (direction == "down")  tiles[(gY + 1) * levelWidth + gX].occupied = true;
        else if (direction == "left")  tiles[gY * levelWidth + gX - 1].occupied = true;
        else if (direction == "right") tiles[gY * levelWidth + gX + 1].occupied = true;
    }
    else if (!inAA && inMA) {
        std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - MAstart;

        MAtimeElapsed = elapsed_seconds.count();
        MAprogress = MAtimeElapsed / c_MAlength;

        // Calculate distance moved while in the MA
        int movedBy;
        movedBy = cellSize - cellSize * (1.0f - MAprogress) * (1.0f - MAprogress);

        // If the barbarian moved halfway to another tile, change his coordinates and update tile lighting info
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

        // Set correct sprites, depending on MA progress
        if (MAprogress < 1.0f) spriteNumber = "1";

        // Calculate current height
        int height = 0;

        // End MA if the progress is 100%
        if (MAprogress >= 1.0f) {
            end_MA();
        }

        // Otherwise, update barbarian position
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

        // Update barbarian sprite position
        sX = cX + spriteOffsetX;
        sY = cY + spriteOffsetY;

        if (inMA) sY -= height;
    }
}

void Lich::end_MA()
{
    inMA            = false;
    changedPosition = false;
    currentAction    = "idle";
    spriteNumber    = "1";

    cX = gX * cellSize + cellSize / 2;
    cY = gY * cellSize + cellSize / 2;
}

void Lich::damage
(
    int amount,
    std::string type,
    Direction direction,
    std::vector<Command> &commands
)
{
    AI::damage(amount, type, direction, commands, true);
}

void Lich::damage
(
    DamageMap damageMap,
    std::vector<Command> &commands
)
{
    AI::damage(damageMap, commands, true);
}