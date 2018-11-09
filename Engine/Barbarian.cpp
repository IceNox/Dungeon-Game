#include "Barbarian.h"

#include "GlobalData.h"
#include "Constants.h"

#include <fstream>
#include <stdexcept>

Barbarian::Barbarian
(
    int gX,
    int gY,
    std::string type,
    std::vector<Command> &commands,
    bool spawn
)
{
    this->gX   = gX;
    this->gY   = gY;
    this->type = type;

    facing        = "left";
    currentAction = "idle";
    spriteFacing  = "left";
    spriteNumber  = "1";

    cX = gX * cellSize + cellSize / 2;
    cY = gY * cellSize + cellSize / 2;

    floating = false;
    onGround = true;

    if (type == "small") {
        health          =  2;
        armor           =  0;
        maxHealth       =  2;
        hitDamage       =  2;
        healthbarHeight =  78;
        spriteOffsetX   = -36;
        spriteOffsetY   = -88;
        hitboxRadius    =  16;
        gold            =  20;

        resistancePhysical = 0.0f;
        resistanceFire     = 0.0f;
        resistanceWater    = 0.0f;
        resistanceMagic    = 0.0f;

        immunePhysical = false;
        immuneFire     = false;
        immuneWater    = false;
        immuneMagic    = false;

        deathParticleColor = Color(255, 178, 127);
    }

    sX = cX + spriteOffsetX;
    sY = cY + spriteOffsetY;

    revealed = false;

    lastAction     = std::chrono::system_clock::now();
    lastTookDamage = std::chrono::system_clock::now();

    if (spawn) {
        gold = 0;

        spawning = true;
        spawnLength = 1.0f;

        //visualEffects.push_back(VisualEffect(0, 0, false, "temp", 0, 0, 0.1f, 10));
        commands.push_back(Command(0, 0, false, "temp", 0, 0, 0.1f, 10, 1.0f));

        spawnStart = system_clock::now();
    }
}

void Barbarian::update_barbarian
(
    Player &player,
    std::vector<Tile> &tiles,
    std::vector<DamageMap> &damageMap,
    std::vector<Command> &commands
)
{
    if (!active) return;

    attack(player, damageMap, commands);
    move(tiles, player);
    update_idle_animation();
}

void Barbarian::attack
(
    Player &player,
    std::vector<DamageMap> &damageMap,
    std::vector<Command> &commands
)
{
    using namespace std::chrono;

    if (!inMA && !inAA) {
        duration<float> elapsed_time = system_clock::now() - lastAction;
        if (elapsed_time < microseconds((int)(actionCooldown * 1000000))) return;

        std::string enemyDirection = "";

        if      (player.gX == gX     && player.gY == gY - 1) enemyDirection = "up";
        else if (player.gX == gX     && player.gY == gY + 1) enemyDirection = "down";
        else if (player.gX == gX - 1 && player.gY == gY)     enemyDirection = "left";
        else if (player.gX == gX + 1 && player.gY == gY)     enemyDirection = "right";
        else throw std::invalid_argument("gX/gY were assigned invalid values");

        if (enemyDirection != "") {
            currentAction = "attack";
            spriteNumber  = "1";

            inAA        = true;
            AAdirection = enemyDirection;
            AAstart     = system_clock::now();

            facing = enemyDirection;
            if      (facing == "left")  spriteFacing = "left";
            else if (facing == "right") spriteFacing = "right";

            actionCooldown = 1.0f;
            lastAction = system_clock::now();
        }
    }

    else if (!inMA && inAA) {

        // Update time
        duration<float> elapsed_seconds = system_clock::now() - AAstart;

        AAtimeElapsed = elapsed_seconds.count();
        AAprogress    = AAtimeElapsed / AAlength;

        // End AA if nescessary
        if (AAprogress >= 1.0f) {
            inAA          = false;
            currentAction = "idle";
            spriteNumber  = "1";

            int index;
            if      (AAdirection == "up")    index = (gY - 1) * levelWidth + gX;
            else if (AAdirection == "down")  index = (gY + 1) * levelWidth + gX;
            else if (AAdirection == "left")  index =  gY      * levelWidth + gX - 1;
            else if (AAdirection == "right") index =  gY      * levelWidth + gX + 1;
            else throw std::invalid_argument("AAdirection was assigned an invalid value");
            
            damageMap[index].active                = true;
            damageMap[index].direction             = NO_DIRECTION;
            damageMap[index].playerDamagePhysical += hitDamage;

            int soX;
            int soY;

            int sprX;
            int sprY = 72;

            if      (AAdirection == "up") {
                soX = 36;
                soY = -48;

                sprX = 8;
            }
            else if (AAdirection == "down") {
                soX = 28;
                soY = 48;

                sprX = 8;
            }
            else if (AAdirection == "left") {
                soX = -40;
                soY = 12;

                sprX = 24;
            }
            else if (AAdirection == "right") {
                soX = 88;
                soY = 12;

                sprX = 24;
            }

            //visualEffects.push_back(VisualEffect(sX + soX, sY + soY, false, "attack_chop_" + AAdirection + "_effect", sprX, sprY, 0.02f, 7));
            commands.push_back(Command(sX + soX, sY + soY, false, "attack_chop_" + AAdirection + "_effect", sprX, sprY, 0.02f, 7, 1.0f));
        }
    }
}

void Barbarian::move
(
    std::vector<Tile> &tiles,
    Player &player
)
{
    using namespace std::chrono;

    if (!inMA && !inAA) {
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
            if      (facing == "up") {
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
            else throw std::invalid_argument("Facing was assigned an invalid value");
        }

        if      (direction == "up") {
            if (tiles[(gY - 1) * levelWidth + gX].occupied) {
                if      (player.gX < gX) direction = "left";
                else if (player.gX > gX) direction = "right";
                else                     direction = "up";
            }
        }
        else if (direction == "down") {
            if (tiles[(gY + 1) * levelWidth + gX].occupied) {
                if      (player.gX < gX) direction = "left";
                else if (player.gX > gX) direction = "right";
                else                     direction = "down";
            }
        }
        else if (direction == "left") {
            if (tiles[gY * levelWidth + gX - 1].occupied) {
                if      (player.gY < gY) direction = "up";
                else if (player.gY > gY) direction = "down";
                else                     direction = "left";
            }
        }
        else if (direction == "right") {
            if (tiles[gY * levelWidth + gX + 1].occupied) {
                if      (player.gY < gY) direction = "up";
                else if (player.gY > gY) direction = "down";
                else                     direction = "right";
            }
        }
        else throw std::invalid_argument("Facing was assigned an invalid value");

        if (direction == "up"    && tiles[(gY - 1) * levelWidth + gX].occupied) return;
        if (direction == "down"  && tiles[(gY + 1) * levelWidth + gX].occupied) return;
        if (direction == "left"  && tiles[ gY * levelWidth + gX - 1].occupied) return;
        if (direction == "right" && tiles[ gY * levelWidth + gX + 1].occupied) return;

        currentAction = "jump";
        spriteNumber  = "1";

        inMA        = true;
        onGround    = false;
        MAdirection = direction;
        MAstart     = system_clock::now();
        sgX         = gX;
        sgY         = gY;
        c_MAlength  = MAlength;

        facing = direction;
        if      (facing == "left")  spriteFacing = "left";
        else if (facing == "right") spriteFacing = "right";

        actionCooldown = 1.0f;
        lastAction = system_clock::now();

        // Set the tile the barbarian is moving to as occupied to prevent other AI / player from entering it
        if      (direction == "up")    tiles[(gY - 1) * levelWidth + gX    ].occupied = true;
        else if (direction == "down")  tiles[(gY + 1) * levelWidth + gX    ].occupied = true;
        else if (direction == "left")  tiles[ gY      * levelWidth + gX - 1].occupied = true;
        else if (direction == "right") tiles[ gY      * levelWidth + gX + 1].occupied = true;
        else throw std::invalid_argument("AAdirection was assigned an invalid value");
    }

    else if (!inAA && inMA) {
        std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - MAstart;
        MAtimeElapsed = elapsed_seconds.count();
        MAprogress    = MAtimeElapsed / c_MAlength;

        // Calculate distance moved while in the MA
        int movedBy = cellSize - static_cast<int>(cellSize * (1.0f - MAprogress) * (1.0f - MAprogress));

        // If the barbarian moved halfway to another tile, change his coordinates
        if (MAprogress > 0.3f && changedPosition == false) {
            changedPosition = true;

            if      (MAdirection == "up")    gY--;
            else if (MAdirection == "down")  gY++;
            else if (MAdirection == "left")  gX--;
            else if (MAdirection == "right") gX++;
            else throw std::invalid_argument("AAdirection was assigned an invalid value");
        }

        // Set correct sprites, depending on MA progress
        if (MAprogress < 0.5f) spriteNumber = "1";
        else                   spriteNumber = "2";

        // Calculate current height
        int height = 10;
        if (!floating) {
            float xAxis = 2 * (MAprogress - 0.5f);
            float yAxis = xAxis * xAxis * -1.0f;

            height = static_cast<int>(ceil(yAxis * jumpHeight) + jumpHeight + 1);
        }

        // End MA if the progress is 100%
        if (MAprogress >= 1.0f) {
            end_MA();
        }

        // Otherwise, update barbarian position
        else {
            if      (MAdirection == "up")    cY = sgY * cellHeight + cellHeight / 2 - movedBy;
            else if (MAdirection == "down")  cY = sgY * cellHeight + cellHeight / 2 + movedBy;
            else if (MAdirection == "left")  cX = sgX * cellWidth  + cellWidth  / 2 - movedBy;
            else if (MAdirection == "right") cX = sgX * cellWidth  + cellWidth  / 2 + movedBy;
            else throw std::invalid_argument("MAdirection was assigned an invalid value");
        }

        // Update barbarian sprite position
        sX = cX + spriteOffsetX;
        sY = cY + spriteOffsetY;

        if (inMA) sY -= height;
    }
}

void Barbarian::end_MA()
{
    inMA            = false;
    onGround        = true;
    changedPosition = false;
    currentAction   = "idle";
    spriteNumber    = "1";

    cX = gX * cellSize + cellSize / 2;
    cY = gY * cellSize + cellSize / 2;
}

void Barbarian::update_idle_animation()
{
    if (!inMA && !inAA) {
        duration<float> elapsed_time = system_clock::now() - lastAction;
        if ((int)(elapsed_time.count() / (actionCooldown / 2)) % 2 == 0) {
            spriteNumber = "1";
        }
        else {
            spriteNumber = "2";
        }
    }
}

void Barbarian::damage
(
    int amount,
    std::string type,
    Direction direction,
    std::vector<Command> &commands
)
{
    AI::damage(amount, type, direction, commands, true);
}

void Barbarian::damage
(
    DamageMap damageMap,
    std::vector<Command> &commands
)
{
    AI::damage(damageMap, commands, true);
}
