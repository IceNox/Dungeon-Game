#include "Entity.h"

#include <sstream>

void DamageProjectile::update()
{
    // Move the entity
    cPos_hp = cPos_hp + (vVec * 1000) * ((maintime::now() - lastMove).get_duration() / 1000000.0f);

    cPos = cPos_hp / 1000;
    gPos = cPos / cellSize;
    sPos = cPos + spriteOffset;

    hitbox.cPos = cPos;

    lastMove = maintime::now();

    // Update animation
    int microsecondsElapsed = (maintime::now() - lastFrameChange).get_duration();
    int holdTimeInMicroseconds = frameHoldTime * 1000;

    while (microsecondsElapsed > holdTimeInMicroseconds) {
        currentFrame = (currentFrame + 1) % spriteCount;

        spriteRegion.left = currentFrame * spriteWidth;
        spriteRegion.right = (currentFrame + 1) * spriteWidth;

        lastFrameChange     += Duration(holdTimeInMicroseconds);
        microsecondsElapsed -= holdTimeInMicroseconds;
    }
}

void HealProjectile::update()
{
    // Move the entity
    cPos_hp = cPos_hp + (vVec * 1000) * ((maintime::now() - lastMove).get_duration() / 1000000.0f);

    cPos = cPos_hp / 1000;
    gPos = cPos / cellSize;
    sPos = cPos + spriteOffset;

    hitbox.cPos = cPos;

    lastMove = maintime::now();

    // Update animation
    int microsecondsElapsed = (maintime::now() - lastFrameChange).get_duration();
    int holdTimeInMicroseconds = frameHoldTime * 1000;

    while (microsecondsElapsed > holdTimeInMicroseconds) {
        currentFrame = (currentFrame + 1) % spriteCount;

        spriteRegion.left = currentFrame * spriteWidth;
        spriteRegion.right = (currentFrame + 1) * spriteWidth;

        lastFrameChange += Duration(holdTimeInMicroseconds);
        microsecondsElapsed -= holdTimeInMicroseconds;
    }
}

void GoldEntity::update()
{
    // Update animation
    int microsecondsElapsed = (maintime::now() - lastFrameChange).get_duration();
    int holdTimeInMicroseconds = frameHoldTime * 1000;

    while (microsecondsElapsed > holdTimeInMicroseconds) {
        currentFrame = (currentFrame + 1) % spriteCount;

        spriteRegion.left = currentFrame * spriteWidth;
        spriteRegion.right = (currentFrame + 1) * spriteWidth;

        lastFrameChange += Duration(holdTimeInMicroseconds);
        microsecondsElapsed -= holdTimeInMicroseconds;
    }
}

void ItemEntity::update()
{
    // Update animation
    int microsecondsElapsed = (maintime::now() - lastFrameChange).get_duration();
    int holdTimeInMicroseconds = frameHoldTime * 1000;

    while (microsecondsElapsed > holdTimeInMicroseconds) {
        currentFrame = (currentFrame + 1) % spriteCount;

        spriteRegion.left = currentFrame * spriteWidth;
        spriteRegion.right = (currentFrame + 1) * spriteWidth;

        lastFrameChange += Duration(holdTimeInMicroseconds);
        microsecondsElapsed -= holdTimeInMicroseconds;
    }
}

// CONSTRUCTORS AHEAD
DamageProjectile::DamageProjectile
(
    Pos2D cPos,
    Pos2D vVec,
    bool cwp,
    bool cwe,
    DamageInfo di,
    Hitbox hb,
    int spri,
    int holdtime
)
{
    this->cPos = cPos;
    this->vVec = vVec;
    gPos = cPos / cellSize;
    cPos_hp = cPos * 1000;

    collidesP = cwp;
    collidesE = cwe;

    damage = di;

    hitbox = hb;
    hitbox.cPos = cPos;

    spriteIndex = spri;
    spriteOffset = { -sprites[spri].GetCenterX(), -sprites[spri].GetCenterY() };
    spriteWidth = sprites[spri].GetWidth() / sprites[spri].GetFrames();
    spriteHeight = sprites[spri].GetHeight();
    spriteCount = sprites[spri].GetFrames();

    if (spriteCount > 1)
        animated = true;
    else
        animated = false;

    currentFrame = 0;
    frameHoldTime = holdtime;
    spriteRegion = { 0, 0, spriteWidth, spriteHeight };
    sPos = cPos + spriteOffset;

    lastFrameChange = maintime::now();
    lastMove = maintime::now();
}

DamageProjectile::DamageProjectile(const LevelMessage &msg, std::string &result)
{
    // Get arguments
    std::vector<std::string> args;
    args.reserve(10);

    split_str(msg.message, args);

    /// Evaluate mandatory arguments
    // Position
    cPos = { str_to_int(args[0]), str_to_int(args[1]) };
    gPos = cPos / cellSize;
    cPos_hp = cPos * 1000;

    // Damage
    damage.amount = str_to_int(args[2]);
    try {
        damage.type = damageTypeMap.at(args[3]);
    }
    catch (...) {
        result = "warning: Invalid damage type, defaulted to physical";
        damage.type = D_PHYSICAL;
    }

    /// Evaluate additional arguments
    if (msg.argKeys.size() > 0) {
        // Visual info
        spriteIndex = msg.int_at("sprite");
        if (spriteIndex < 0 || spriteIndex >= sprites.size()) spriteIndex = EMPTY_SPRITE;

        int ht = msg.int_at("fholdtime");
        if (ht != -1)
            frameHoldTime = ht;
        else
            frameHoldTime = 150;

        // Velocity
        std::string vec = msg.str_at("velocity");
        if (vec.length() > 0) {
            std::vector<std::string> vals;
            split_str(vec, vals, ';');

            if (vals.size() == 2) {
                vVec = { str_to_int(vals[0]), str_to_int(vals[1]) };
            }
        }
        else {
            vVec = { 0, 0 };
        }

        // Collisions
        collidesP = msg.int_at("collidesp");
        collidesE = msg.int_at("collidese");

        // Hitbox
        std::string hbx = msg.str_at("hitbox");
        if (hbx.length() > 0) {
            std::vector<std::string> vals;
            split_str(hbx, vals, ';');

            if (vals.size() == 4) {
                hitbox.type = str_to_int(vals[0]);
                hitbox.radius = str_to_int(vals[1]);
                hitbox.width = str_to_int(vals[2]);
                hitbox.height = str_to_int(vals[3]);
            }
            else {
                hitbox.active = false;
            }
        }
        else {
            hitbox.active = false;
        }

        // Additional damage arguments
        damage.precise = msg.int_at("precise");
        damage.knockbackStr = msg.int_at("knockback");

        std::vector<std::string> effects;
        split_str(msg.str_at("effects"), effects, '|');
        damage.statusEffects = effects;
    }

    hitbox.cPos = cPos;

    spriteOffset = { -sprites[spriteIndex].GetCenterX(), -sprites[spriteIndex].GetCenterY() };
    spriteWidth = sprites[spriteIndex].GetWidth() / sprites[spriteIndex].GetFrames();
    spriteHeight = sprites[spriteIndex].GetHeight();
    spriteCount = sprites[spriteIndex].GetFrames();

    if (spriteCount > 1)
        animated = true;
    else
        animated = false;

    currentFrame = 0;
    spriteRegion = { 0, 0, spriteWidth, spriteHeight };
    sPos = cPos + spriteOffset;

    lastFrameChange = maintime::now();
    lastMove = maintime::now();
}

HealProjectile::HealProjectile
(
    Pos2D cPos,
    Pos2D vVec,
    bool cwp,
    bool cwe,
    HealInfo hi,
    Hitbox hb,
    int spri,
    int holdtime
)
{
    this->cPos = cPos;
    this->vVec = vVec;
    gPos = cPos / cellSize;
    cPos_hp = cPos * 1000;

    collidesP = cwp;
    collidesE = cwe;

    heal = hi;

    hitbox = hb;
    hitbox.cPos = cPos;

    spriteIndex = spri;
    spriteOffset = { -sprites[spri].GetCenterX(), -sprites[spri].GetCenterY() };
    spriteWidth = sprites[spri].GetWidth() / sprites[spri].GetFrames();
    spriteHeight = sprites[spri].GetHeight();
    spriteCount = sprites[spri].GetFrames();

    if (spriteCount > 1)
        animated = true;
    else
        animated = false;

    currentFrame = 0;
    frameHoldTime = holdtime;
    spriteRegion = { 0, 0, spriteWidth, spriteHeight };
    sPos = cPos + spriteOffset;

    lastFrameChange = maintime::now();
    lastMove = maintime::now();
}

HealProjectile::HealProjectile(const LevelMessage &msg, std::string &result)
{
    // Get arguments
    std::vector<std::string> args;
    args.reserve(10);

    split_str(msg.message, args);

    /// Evaluate mandatory arguments
    // Position
    cPos = { str_to_int(args[0]), str_to_int(args[1]) };
    gPos = cPos / cellSize;
    cPos_hp = cPos * 1000;

    // Heal
    heal.amount = str_to_int(args[2]);

    /// Evaluate additional arguments
    if (msg.argKeys.size() > 0) {
        // Visual info
        spriteIndex = msg.int_at("sprite");
        if (spriteIndex < 0 || spriteIndex >= sprites.size()) spriteIndex = EMPTY_SPRITE;

        int ht = msg.int_at("fholdtime");
        if (ht != -1)
            frameHoldTime = ht;
        else
            frameHoldTime = 150;

        // Velocity
        std::string vec = msg.str_at("velocity");
        if (vec.length() > 0) {
            std::vector<std::string> vals;
            split_str(vec, vals, ';');

            if (vals.size() == 2) {
                vVec = { str_to_int(vals[0]), str_to_int(vals[1]) };
            }
        }
        else {
            vVec = { 0, 0 };
        }

        // Collisions
        collidesP = msg.int_at("collidesp");
        collidesE = msg.int_at("collidese");

        // Hitbox
        std::string hbx = msg.str_at("hitbox");
        if (hbx.length() > 0) {
            std::vector<std::string> vals;
            split_str(hbx, vals, ';');

            if (vals.size() == 4) {
                hitbox.type = str_to_int(vals[0]);
                hitbox.radius = str_to_int(vals[1]);
                hitbox.width = str_to_int(vals[2]);
                hitbox.height = str_to_int(vals[3]);
            }
            else {
                hitbox.active = false;
            }
        }
        else {
            hitbox.active = false;
        }
    }

    hitbox.cPos = cPos;

    spriteOffset = { -sprites[spriteIndex].GetCenterX(), -sprites[spriteIndex].GetCenterY() };
    spriteWidth = sprites[spriteIndex].GetWidth() / sprites[spriteIndex].GetFrames();
    spriteHeight = sprites[spriteIndex].GetHeight();
    spriteCount = sprites[spriteIndex].GetFrames();

    if (spriteCount > 1)
        animated = true;
    else
        animated = false;

    currentFrame = 0;
    spriteRegion = { 0, 0, spriteWidth, spriteHeight };
    sPos = cPos + spriteOffset;

    lastFrameChange = maintime::now();
    lastMove = maintime::now();
}

GoldEntity::GoldEntity(Pos2D gPos, int amount)
{
    this->gPos = gPos;
    this->amount = amount;

    cPos = gPos * cellSize + Pos2D(cellSize / 2) + Pos2D(0, -1);
    cPos_hp = cPos * 1000;

    collidesP = true;
    collidesE = false;

    hitbox.active = true;
    hitbox.type = HITBOX_CIRCLE;
    hitbox.cPos = cPos;

    if (amount > 49) {
        spriteIndex = GOLD_7;
        hitbox.radius = 40;
    }
    else if (amount > 19) {
        spriteIndex = GOLD_6;
        hitbox.radius = 28;
    }
    else if (amount > 9) {
        spriteIndex = GOLD_5;
        hitbox.radius = 28;
    }
    else if (amount > 4) {
        spriteIndex = GOLD_4;
        hitbox.radius = 24;
    }
    else if (amount > 2) {
        spriteIndex = GOLD_3;
        hitbox.radius = 20;
    }
    else if (amount > 1) {
        spriteIndex = GOLD_2;
        hitbox.radius = 20;
    }
    else if (amount > 0) {
        spriteIndex = GOLD_1;
        hitbox.radius = 12;
    }
    else {
        spriteIndex = EMPTY_SPRITE;
    }

    spriteOffset = { -sprites[spriteIndex].GetCenterX(), -sprites[spriteIndex].GetCenterY() };
    sPos = cPos + spriteOffset;
}

GoldEntity::GoldEntity(const LevelMessage &msg, std::string &result)
{
    // Get arguments
    std::vector<std::string> args;
    args.reserve(10);

    split_str(msg.message, args);

    /// Evaluate mandatory arguments
    // Position
    gPos = { str_to_int(args[0]), str_to_int(args[1]) };
    cPos = gPos * cellSize + Pos2D(cellSize / 2) + Pos2D(0, -1);
    cPos_hp = cPos * 1000;

    // Amount
    amount = str_to_int(args[2]);


    collidesP = true;
    collidesE = false;

    hitbox.active = true;
    hitbox.type = HITBOX_CIRCLE;
    hitbox.cPos = cPos;

    if (amount > 49) {
        spriteIndex = GOLD_7;
        hitbox.radius = 40;
    }
    else if (amount > 19) {
        spriteIndex = GOLD_6;
        hitbox.radius = 28;
    }
    else if (amount > 9) {
        spriteIndex = GOLD_5;
        hitbox.radius = 28;
    }
    else if (amount > 4) {
        spriteIndex = GOLD_4;
        hitbox.radius = 24;
    }
    else if (amount > 2) {
        spriteIndex = GOLD_3;
        hitbox.radius = 20;
    }
    else if (amount > 1) {
        spriteIndex = GOLD_2;
        hitbox.radius = 20;
    }
    else if (amount > 0) {
        spriteIndex = GOLD_1;
        hitbox.radius = 12;
    }
    else {
        spriteIndex = EMPTY_SPRITE;
    }

    spriteOffset = { -sprites[spriteIndex].GetCenterX(), -sprites[spriteIndex].GetCenterY() };
    sPos = cPos + spriteOffset;
}

/*
Entity::Entity // Constructor for a regular entity
(
    Pos2D cPos,
    Pos2D vVec,
    Pos2D sOffset,
    Pos2D rangeVec,
    int            damage,
    int            heal,
    int            hitboxRadius,
    bool        cwp,
    bool        cwa,
    bool        animated,
    SpriteName    spriteName,
    int            sW,
    int            sH,
    int            frameCount,
    int            frameHoldTime
)    :
    cPos(cPos),
    vVec(vVec),
    sOffset(sOffset),
    rangeVec(rangeVec),
    damage(damage),
    heal(heal),
    hitboxRadius(hitboxRadius),
    collidesWithPlayer(cwp),
    collidesWithAI(cwa),
    animated(animated),
    spriteName(spriteName),
    sWidth(sW),
    sHeight(sH),
    frameCount(frameCount),
    frameHoldTime(frameHoldTime)
{
    gPos = cPos / cellSize;
    sPos = cPos - sOffset;

    scPos = cPos;

    item = false;
    gold = false;

    itemName = "";

    goldAmount = 0;

    currentFrame = 0;

    spriteRegion = { 0, 0, sWidth, sHeight };

    revealed = false;
    visible  = false;

    creationTime    = maintime::now();
    lastMove        = maintime::now();
    lastFrameChange = maintime::now();
}

Entity::Entity // Constructor for gold entity
(
    Pos2D gPos,
    int    goldAmount
)    :
    gPos(gPos),
    goldAmount(goldAmount)
{
    gold = true;
    item = false;

    collidesWithPlayer = true;
    collidesWithAI       = false;

    cPos = gPos * cellSize;
    cPos.x += cellSize / 2;
    cPos.y += cellSize / 2 - 1;

    if (goldAmount > 0) {
        spriteName = GOLD_1;

        hitboxRadius = 12;
        sOffset = Pos2D(-12, -12);
    }
    if (goldAmount > 1) {
        spriteName = GOLD_2;

        hitboxRadius = 20;
        sOffset = Pos2D(-20, -16);
    }
    if (goldAmount > 2) {
        spriteName = GOLD_3;

        hitboxRadius = 20;
        sOffset = Pos2D(-20, -24);
    }
    if (goldAmount > 4) {
        spriteName = GOLD_4;

        hitboxRadius = 24;
        sOffset = Pos2D(-24, -28);
    }
    if (goldAmount > 9) {
        spriteName = GOLD_5;

        hitboxRadius = 28;
        sOffset = Pos2D(-28, -36);
    }
    if (goldAmount > 19) {
        spriteName = GOLD_6;

        hitboxRadius = 28;
        sOffset = Pos2D(-28, -36);
    }
    if (goldAmount > 49) {
        spriteName = GOLD_7;

        hitboxRadius = 40;
        sOffset = Pos2D(-40, -48);
    }

    sPos = cPos + sOffset;

    visible  = false;
    revealed = false;
    animated = false;
}

Entity::Entity // Constructor for item entity
(
    Pos2D gPos,
    SpriteName itemName
)   :
    gPos(gPos),
    spriteName(itemName)
{
    gold = false;
    item = true;

    collidesWithPlayer = true;
    collidesWithAI = false;

    cPos = gPos * cellSize;
    cPos.x += cellSize / 2;
    cPos.y += cellSize / 2 - 1;

    sOffset = Pos2D(-32, -48);

    sPos = cPos + sOffset;

    visible = false;
    revealed = false;
    animated = false;
}

void Entity::update_entity()
{
    if (!item && !gold) {
        // Move the entity
        cPos = cPos + vVec * ((maintime::now() - lastMove).get_duration() / 1000000.0f);
        gPos = cPos / cellSize;
        sPos = cPos + sOffset;

        //if ((cX + cellWidth / 2) / cellWidth < gX || (cX + cellWidth / 2) / cellWidth > gX) gX = cX / cellWidth;
        //if ((cY + cellHeight / 2) / cellHeight < gY || (cY + cellHeight / 2) / cellHeight > gY) gY = cY / cellHeight;
        lastMove = maintime::now();

        // Update animation
        if (!animated) return;

        int microsecondsElapsed       = (maintime::now() - lastFrameChange).get_duration();
        int holdTimeInMicroseconds = frameHoldTime * 1000;

        while (microsecondsElapsed > holdTimeInMicroseconds) {
            currentFrame = (currentFrame + 1) % frameCount;

            spriteRegion.left  =  currentFrame        * sWidth;
            spriteRegion.right = (currentFrame + 1) * sWidth;
            /*
            std::string spriteNumber = "_";
            std::stringstream ss;
            ss << currentFrame + 1;
            spriteNumber += ss.str();

            spriteName = baseSpriteName + spriteNumber;

            lastFrameChange        += Duration(holdTimeInMicroseconds);
            microsecondsElapsed -= holdTimeInMicroseconds;
        }
    }
}
*/
