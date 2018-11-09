#include "Shooter.h"

Shooter::Shooter(
    int gX,
    int gY,
    std::string type,
    int offset,
    std::string facing,
    bool active
    )
{
    this->type = type;
    this->gX = gX;
    this->gY = gY;
    this->offset = offset;
    this->facing = facing;
    this->active = active;
    this->floating = false;
    this->onGround = true;

    if (type == "small") {
        this->health = 4;
        this->armor = 0;
        this->maxHealth = 4;
        this->healthbarHeight = 78;
        //this->resistance = 0.0f;
        this->spriteOffsetX = -48;
        this->spriteOffsetY = -78;
        this->hitboxRadius = 40;
        this->bulletDamage = 2;
        this->bulletSpeed = 500;
        this->cooldown = 2000;
        this->range = 100000;
        this->bulletHitboxRadius = 8;
        this->bulletSpriteOffsetX = -8;
        this->bulletSpriteOffsetY = -30;
    }
    else if (type == "normal") {
        this->health = 5;
        this->armor = 0;
        this->maxHealth = 5;
        this->healthbarHeight = 80;
        //this->resistance = 0.0f;
        this->spriteOffsetX = -48; /// SET THIS
        this->spriteOffsetY = -68; /// SET THIS
        this->hitboxRadius = 48; /// SET THIS
        this->bulletDamage = 4;
        this->bulletSpeed = 400;
        this->cooldown = 2000;
        this->range = 100000;
        this->bulletHitboxRadius = 18; /// SET THIS
        this->bulletSpriteOffsetX = -46; /// SET THIS
        this->bulletSpriteOffsetY = -70; /// SET THIS
    }
    else if (type == "big") {
        this->health = 10;
        this->armor = 0;
        this->maxHealth = 10;
        this->healthbarHeight = 40;
        //this->resistance = 0.0f;
        this->spriteOffsetX = 0; /// SET THIS
        this->spriteOffsetY = 0; /// SET THIS
        this->hitboxRadius = 0; /// SET THIS
        this->bulletDamage = 8;
        this->bulletSpeed = 500;
        this->cooldown = 2000;
        this->range = 100000;
        this->bulletHitboxRadius = 0; /// SET THIS
        this->bulletSpriteOffsetX = 0; /// SET THIS
        this->bulletSpriteOffsetY = 0; /// SET THIS
    }

    cX = gX * cellWidth + cellWidth / 2;
    cY = gY * cellHeight + cellHeight / 2;

    sX = cX + spriteOffsetX;
    sY = cY + spriteOffsetY;

    revealed = false;

    using namespace std::chrono;

    creationTime = system_clock::now();
    lastShotTime = creationTime + milliseconds(this->offset) - milliseconds(this->cooldown);
}

void Shooter::update_shooter(std::vector<Entity> &entities, int &entityCount)
{
    shoot(entities, entityCount);

    if (health == 0) dead = true;
}

void Shooter::shoot(std::vector<Entity> &entities, int &entityCount)
{
    using namespace std::chrono;

    time_point<system_clock> now = system_clock::now();
    duration<float> elapsed_time = now - lastShotTime;

    while (elapsed_time >= milliseconds(cooldown)) {
        elapsed_time -= milliseconds(cooldown);
        lastShotTime += milliseconds(cooldown);

        duration<float> bulletAirTime = now - lastShotTime;

        int coordOffset = bulletHitboxRadius + hitboxRadius;

        int tx = cX - (gX * cellWidth);
        int ty = cY - (gY * cellHeight);

        if (facing == "up") {
            std::string names[4];
            if        (type == "small") {
                names[0] = "small_cannonball_1_left_right";
                names[1] = "small_cannonball_2_left_right";
                names[2] = "small_cannonball_1_left_right";
                names[3] = "small_cannonball_2_left_right";
            }
            else if (type == "normal") {
                names[0] = "normal_cannonball_1_left_right";
                names[1] = "normal_cannonball_2_left_right";
                names[2] = "normal_cannonball_1_left_right";
                names[3] = "normal_cannonball_2_left_right";
            }
            else if (type == "big") {
                names[0] = "big_cannonball_1_left_right";
                names[1] = "big_cannonball_2_left_right";
                names[2] = "big_cannonball_1_left_right";
                names[3] = "big_cannonball_2_left_right";
            }

            ty -= coordOffset;
            if (ty < 0) {
                //entities.push_back(Entity(gX, gY - 1, tx, cellHeight - 1 + ty, bulletDamage, 0, bulletHitboxRadius, bulletSpriteOffsetX, bulletSpriteOffsetY, true, false, 0, -bulletSpeed, range, range, true, 4, names, 0.15f));
            }
            else {
                //entities.push_back(Entity(gX, gY, tx, ty, bulletDamage, 0, bulletHitboxRadius, bulletSpriteOffsetX, bulletSpriteOffsetY, true, false, 0, -bulletSpeed, range, range, true, 4, names, 0.15f));
            }
        }
        else if (facing == "down") {
            std::string names[4];

            if        (type == "small") {
                names[0] = "small_cannonball_1_left_right";
                names[1] = "small_cannonball_2_left_right";
                names[2] = "small_cannonball_1_left_right";
                names[3] = "small_cannonball_2_left_right";
            }
            else if (type == "normal") {
                names[0] = "normal_cannonball_1_left_right";
                names[1] = "normal_cannonball_2_left_right";
                names[2] = "normal_cannonball_1_left_right";
                names[3] = "normal_cannonball_2_left_right";
            }
            else if (type == "big") {
                names[0] = "big_cannonball_1_left_right";
                names[1] = "big_cannonball_2_left_right";
                names[2] = "big_cannonball_1_left_right";
                names[3] = "big_cannonball_2_left_right";
            }

            ty += coordOffset;
            if (ty > cellHeight - 1) {
                //entities.push_back(Entity(gX, gY + 1, tx, ty - cellHeight, bulletDamage, 0, bulletHitboxRadius, bulletSpriteOffsetX, bulletSpriteOffsetY, true, false, 0, bulletSpeed, range, range, true, 4, names, 0.25f));
            }
            else {
                //entities.push_back(Entity(gX, gY, tx, ty, bulletDamage, 0, bulletHitboxRadius, bulletSpriteOffsetX, bulletSpriteOffsetY, true, false, 0, bulletSpeed, range, range, true, 4, names, 0.25f));
            }
        }
        else if (facing == "left") {
            std::string names[4];

            if        (type == "small") {
                names[0] = "small_cannonball_4_left_right";
                names[1] = "small_cannonball_3_left_right";
                names[2] = "small_cannonball_2_left_right";
                names[3] = "small_cannonball_1_left_right";
            }
            else if (type == "normal") {
                names[0] = "normal_cannonball_4_left_right";
                names[1] = "normal_cannonball_3_left_right";
                names[2] = "normal_cannonball_2_left_right";
                names[3] = "normal_cannonball_1_left_right";
            }
            else if (type == "big") {
                names[0] = "big_cannonball_4_left_right";
                names[1] = "big_cannonball_3_left_right";
                names[2] = "big_cannonball_2_left_right";
                names[3] = "big_cannonball_1_left_right";
            }

            tx -= coordOffset;
            if (tx < 0) {
                //entities.push_back(Entity(gX - 1, gY, cellWidth - 1 + tx, ty, bulletDamage, 0, bulletHitboxRadius, bulletSpriteOffsetX, bulletSpriteOffsetY, true, false, -bulletSpeed, 0, range, range, true, 4, names, 0.25f));
            }
            else {
                //entities.push_back(Entity(gX, gY, tx, ty, bulletDamage, 0, bulletHitboxRadius, bulletSpriteOffsetX, bulletSpriteOffsetY, true, false, -bulletSpeed, 0, range, range, true, 4, names, 0.25f));
            }
        }
        else if (facing == "right") {
            std::string names[4];

            if        (type == "small") {
                names[0] = "small_cannonball_1_left_right";
                names[1] = "small_cannonball_2_left_right";
                names[2] = "small_cannonball_3_left_right";
                names[3] = "small_cannonball_4_left_right";
            }
            else if (type == "normal") {
                names[0] = "normal_cannonball_1_left_right";
                names[1] = "normal_cannonball_2_left_right";
                names[2] = "normal_cannonball_3_left_right";
                names[3] = "normal_cannonball_4_left_right";
            }
            else if (type == "big") {
                names[0] = "big_cannonball_1_left_right";
                names[1] = "big_cannonball_2_left_right";
                names[2] = "big_cannonball_3_left_right";
                names[3] = "big_cannonball_4_left_right";
            }

            tx += coordOffset;
            if (tx > cellWidth - 1) {
                //entities.push_back(Entity(gX + 1, gY, tx - cellWidth, ty, bulletDamage, 0, bulletHitboxRadius, bulletSpriteOffsetX, bulletSpriteOffsetY, true, false, bulletSpeed, 0, range, range, true, 4, names, 0.15f));
            }
            else {
                //entities.push_back(Entity(gX, gY, tx, ty, bulletDamage, 0, bulletHitboxRadius, bulletSpriteOffsetX, bulletSpriteOffsetY, true, false, bulletSpeed, 0, range, range, true, 4, names, 0.15f));
            }
        }

        //entities[entityCount].lastMove = lastShotTime;
        entities[entityCount].update_entity();
        entityCount++;
    }
}

void Shooter::damage(int amount)
{
    if (amount == 0) return;

    int reduce = 0;
    //if (resistance == 0.0f) reduce = 0;
    //else reduce = ceil(resistance);

    if (amount - reduce < 1) amount = 1;
    else amount -= reduce;

    healthbarVisible = true;

    if (armor > 0) {
        if (armor - amount < 0) {
            amount -= armor;
            armor = 0;
            health -= amount;
            return;
        }
        else {
            armor -= amount;
            return;
        }
    }

    health -= amount;
    if (health < 0) health = 0;
    if (health == 0) healthbarVisible = false;
}