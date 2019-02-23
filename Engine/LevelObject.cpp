#include "LevelObject.h"

#include "GlobalData.h"
#include "Functions.h"
#include "LevelMessage.h"

#include <exception>

#include <fstream>

void LevelObject::move_to(Pos2D pos, int duration)
{
    if (pos == gPos) return;

    moveState.ongoing  = true;
    moveState.duration = duration;
    moveState.progress = 0.0f;
    moveState.start    = maintime::now();

    moveStartPos = cPos;
    moveEndPos   = pos * cellSize + cellSize / 2;

    if (duration != 0) {
        moveDirection = (pos - gPos).vdirection();

        if (moveDirection != UP && moveDirection != DOWN) {
            facing = moveDirection;
            (*scripts[scri])["facing"] = (int)facing;
        }
    }
}

void LevelObject::move_by(Pos2D vec, int duration)
{
    move_to(gPos + vec, duration);
}

void LevelObject::move_to_(int x, int y, int duration)
{
    move_to({ x, y }, duration);
}

void LevelObject::move_by_(int x, int y, int duration)
{
    move_by({ x, y }, duration);
}

void LevelObject::update_movement(const LevelStateData& ld)
{
    // Get time elapsed
    int mselapsed = (maintime::now() - moveState.start).get_duration(MILLISECONDS);
    moveState.progress = (double)mselapsed / moveState.duration;

    // End movement state if necessary
    if (moveState.progress >= 1.0f) {
        cPos = moveEndPos;
        height = 0;

        moveState.ongoing = false;
        (*scripts[scri])["reset_movement"]();

        return;
    }

    // Calculate position
    float movedBy = 0.0f;
    float mheight = 0.0f;

    // Default move path
    if ((*scripts[scri])["movement"]["mtype"] == -1) {
        movedBy = moveState.progress * (2.0f - moveState.progress);

        float xAxis = 2 * (moveState.progress - 0.5f);
        float yAxis = xAxis * xAxis * -1.0f;

        mheight = yAxis * 30.0f + 30.0f;
    }
    // Script calculated move path
    else {
        sol::tie(movedBy, mheight) = (*scripts[scri])["movementtypes"][(*scripts[scri])["movement"]["mtype"]](moveState.progress);
    }

    Pos2D moveVec = moveEndPos - moveStartPos;
    Pos2D newcPos = moveStartPos + (moveVec * movedBy);

    // Check for collision with an occupied tile
    Pos2D newtPos = newcPos - (gPos * cellSize);
    bool collided = false;
    const int C_DIST = 16;
    if (newtPos.x < C_DIST) { // Left
        int index = gPos.y * _LEVEL_WIDTH + (gPos.x - 1);
        collided = ld.tiles[index].occupied;
    }
    else if (newtPos.y < C_DIST) { // Up
        int index = (gPos.y - 1) * _LEVEL_WIDTH + gPos.x;
        collided = ld.tiles[index].occupied;
    }
    else if (newtPos.x > cellSize - C_DIST) { // Right
        int index = gPos.y * _LEVEL_WIDTH + (gPos.x + 1);
        collided = ld.tiles[index].occupied;
    }
    else if (newtPos.y > cellSize - C_DIST) { // Down
        int index = (gPos.y + 1) * _LEVEL_WIDTH + gPos.x;
        collided = ld.tiles[index].occupied;
    }

    if (collided) {
        Pos2D gcPos = gPos * cellSize + Pos2D(cellSize / 2);

        float ratio = 1.0f / (1.0f - moveState.progress);
        Pos2D s2Pos = gcPos + (cPos - gcPos) * ratio;

        moveStartPos = s2Pos;
        moveEndPos = gcPos;
        moveVec = moveEndPos - moveStartPos;

        newcPos = moveStartPos + (moveVec * movedBy);
    }

    cPos = moveStartPos + moveVec * movedBy;
    height = ceil(mheight);
}

StaticObject::StaticObject(int id, Pos2D gPos, std::string data)
{
    // Set essential data
    this->oid = id;
    this->ID = oid / 1000;
    this->VR = oid % 1000;

    this->gPos = gPos;

    cPos = gPos * cellSize + Pos2D(cellSize / 2);

    // Get script index
    scri = scripts.AddState();

    // Set up script state
    scripts[scri]->open_libraries(sol::lib::base);
    scripts[scri]->open_libraries(sol::lib::math);

    scripts[scri]->new_usertype<Pos2D>
    (
        "position",
        sol::constructors<Pos2D()>(),
        "x", &Pos2D::x,
        "y", &Pos2D::y
    );

    scripts[scri]->new_usertype<TileData>
    (
        "tileData",
        sol::constructors<>(),
        "pressuredW" , sol::readonly(&TileData::pressuredW),
        "pressuredS" , sol::readonly(&TileData::pressuredS),
        "pressuredG" , sol::readonly(&TileData::pressuredG),
        "pressuredB" , sol::readonly(&TileData::pressuredB),
        "pressured"  , sol::readonly(&TileData::pressured),
        "powered"    , sol::readonly(&TileData::powered),
        "occupied"   , sol::readonly(&TileData::occupied),
        "terrain"    , sol::readonly(&TileData::terrain),
        "object"     , sol::readonly(&TileData::object),
        "player"     , sol::readonly(&TileData::player),
        "enemy"      , sol::readonly(&TileData::enemy),
        "playerid"   , sol::readonly(&TileData::playerId),
        "enemyid"    , sol::readonly(&TileData::enemyId),
        "directlyLit", sol::readonly(&TileData::directlyLit)
    );

    scripts[scri]->new_usertype<LevelStateData>
    (
        "levelData",
        sol::constructors<>(),
        "width"      , sol::readonly(&LevelStateData::width),
        "height"     , sol::readonly(&LevelStateData::height),
        "playercount", sol::readonly(&LevelStateData::playerCount),
        "playerx"    , sol::readonly(&LevelStateData::playerX),
        "playery"    , sol::readonly(&LevelStateData::playerY),
        "tiles"      , sol::readonly(&LevelStateData::tiles)
    );

    scripts[scri]->new_usertype<DamageInfo>
    (
        "damageInfo",
        sol::constructors<>(),
        "amount"       , sol::readonly(&DamageInfo::amount),
        "type"         , sol::readonly(&DamageInfo::type),
        "players"      , sol::readonly(&DamageInfo::players),
        "enemies"      , sol::readonly(&DamageInfo::enemies),
        "terrain"      , sol::readonly(&DamageInfo::terrain),
        "direction"    , sol::readonly(&DamageInfo::dir),
        "ground"       , sol::readonly(&DamageInfo::ground),
        "precise"      , sol::readonly(&DamageInfo::precise),
        "knockbackStr" , sol::readonly(&DamageInfo::knockbackStr),
        "statusEffects", sol::readonly(&DamageInfo::statusEffects)
    );

    scripts[scri]->new_usertype<LevelMessage>
    (
        "levelMessage",
        sol::constructors<LevelMessage()>(),
        "source"    , &LevelMessage::source,
        "message"   , &LevelMessage::message,
        "argKeys"   , &LevelMessage::argKeys,
        "argValsInt", &LevelMessage::argValsInt,
        "argValsStr", &LevelMessage::argValsStr
    );

    // Link object to apropriate script
    int linked = false;
    for (int i = 0; i < _OBJECT_SCRIPTS.size(); i++) {
        if (ID == _OBJECT_SCRIPTS[i].id) {
            // Open script file
            scripts[scri]->script_file(_OBJECT_SCRIPTS[i].filePath);

            // Set variant
            (*scripts[scri])["VARIANT"] = VR;

            // Set sprites
            for (int j = 0; j < _OBJECT_SCRIPTS[i].spriteIndexes.size(); j++) {
                (*scripts[scri])["spriteindexes"][j+1] = _OBJECT_SCRIPTS[i].spriteIndexes[j];
            }

            linked = true;
            break;
        }
    }

    // Destroy object if no script is found
    if (!linked) {
        destroyed = true;
        return;
    }

    // Deserialize data
    if (data.length() > 0) {
        (*scripts[scri])["deserialize"](data);
    }

    // Set health variables
    maxHealth = (*scripts[scri])["maxhealth"];
    health = (*scripts[scri])["health"];
    armor = (*scripts[scri])["armor"];

    healthbarHeight = (*scripts[scri])["healthbarheight"];
    healthbarVisible = (*scripts[scri])["healthbarvisible"];

    // Set sprite variables
    currentSprite = EMPTY_SPRITE;
    sOffset = { -sprites[currentSprite].GetCenterX(), -sprites[currentSprite].GetCenterY() };
    sGround = (*scripts[scri])["ground"];

    // Set hitbox
    sol::optional<int> hType = (*scripts[scri])["hitbox"]["shape"];
    hitbox.type = hType.value();

    sol::optional<int> w = (*scripts[scri])["hitbox"]["size"]["w"];
    sol::optional<int> h = (*scripts[scri])["hitbox"]["size"]["h"];
    sol::optional<int> r = (*scripts[scri])["hitbox"]["size"]["r"];

    hitbox.width  = w.value();
    hitbox.height = h.value();
    hitbox.radius = r.value();
    
    sol::optional<int> hox = (*scripts[scri])["hitbox"]["offset"]["x"];
    sol::optional<int> hoy = (*scripts[scri])["hitbox"]["offset"]["y"];

    hOffset = { hox.value(), hoy.value() };

    // Set light source
    sol::optional<int> isLS = (*scripts[scri])["lightsourcecount"];
    lightSource = isLS.value();

    for (int i = 0; i < isLS.value(); i++) {
        sol::optional<int> lsX = (*scripts[scri])["lightsources"][i]["offset"]["x"];
        sol::optional<int> lsY = (*scripts[scri])["lightsources"][i]["offset"]["y"];
        lightSourcePos.push_back(Pos2D(lsX.value(), lsY.value()));

        sol::optional<int> str = (*scripts[scri])["lightsources"][i]["strength"];
    }
}

void StaticObject::update(std::vector<LevelMessage> &messages, const LevelStateData &ld, long int curTime)
{
    // Set variables
    (*scripts[scri])["moving"] = moveState.ongoing;
    (*scripts[scri])["moveprogress"] = moveState.progress;

    // Run logic
    (*scripts[scri])["update"](cPos, ld, maintime::currentGameTime / 1000);

    if (!moveState.ongoing) {
        // Check for a movement call
        if ((*scripts[scri])["startmovement"] == true) {
            if ((*scripts[scri])["movement"]["relative"] == true) {
                move_by_(
                    (*scripts[scri])["movement"]["x"],
                    (*scripts[scri])["movement"]["y"],
                    (*scripts[scri])["movement"]["duration"]
                );
            }
            else {
                move_to_(
                    (*scripts[scri])["movement"]["x"],
                    (*scripts[scri])["movement"]["y"],
                    (*scripts[scri])["movement"]["duration"]
                );
            }
        }
    }
    else {
        update_movement(ld);
    }

    // Get messages
    int count = (*scripts[scri])["messagecount"].get<int>();
    for (int i = 0; i < count; i++) {
        messages.push_back((*scripts[scri])["get_message"](i));
    }
    (*scripts[scri])["clear_messages"]();

    // Set grid position
    gPos = cPos / cellSize;

    // Set sprite
    currentSprite = (*scripts[scri])["select_sprite"](maintime::currentGameTime / 1000);
    sOffset = { -sprites[currentSprite].GetCenterX(), -sprites[currentSprite].GetCenterY() };
    sPos = cPos + sOffset;
    sPos.y -= height;
    sGround = (*scripts[scri])["ground"];

    // Set hitbox
    hitbox.cPos = cPos;

    // Set other variables
    sol::optional<int> trs = (*scripts[scri])["transparent"];
    sol::optional<int> obs = (*scripts[scri])["obstructive"];
    sol::optional<int> pre = (*scripts[scri])["pressuring"];
    sol::optional<int> pwr = (*scripts[scri])["powering"];
    transparent = trs.value();
    obstructive = obs.value();
    pressuring = pre.value();
    powering = pwr.value();

    facing = static_cast<Direction>((*scripts[scri])["facing"]);

    return;
}

void StaticObject::damage(std::vector<LevelMessage> &messages, const DamageInfo &di)
{
    destroyed = (*scripts[scri])["damage"](di, destroyed);

    if (!moveState.ongoing) {
        // Check for a movement call
        if ((*scripts[scri])["startmovement"] == true) {
            if ((*scripts[scri])["movement"]["relative"] == true) {
                move_by_(
                    (*scripts[scri])["movement"]["x"],
                    (*scripts[scri])["movement"]["y"],
                    (*scripts[scri])["movement"]["duration"]
                );
            }
            else {
                move_to_(
                    (*scripts[scri])["movement"]["x"],
                    (*scripts[scri])["movement"]["y"],
                    (*scripts[scri])["movement"]["duration"]
                );
            }
        }
    }
}

void StaticObject::destroy()
{
    scripts.ReleaseObj(scri);
}

DynamicObject::DynamicObject(int id, Pos2D gPos, std::string data)
{
    // Set essential data
    this->oid = id;
    this->ID = oid / 1000;
    this->VR = oid % 1000;

    this->gPos = gPos;

    cPos = gPos * cellSize + Pos2D(cellSize / 2);

    // Get script index
    scri = scripts.AddState();

    // Set up script state
    scripts[scri]->open_libraries(sol::lib::base);
    scripts[scri]->open_libraries(sol::lib::math);

    scripts[scri]->new_usertype<Pos2D>
    (
        "position",
        sol::constructors<Pos2D()>(),
        "x", &Pos2D::x,
        "y", &Pos2D::y
    );

    scripts[scri]->new_usertype<TileData>
    (
        "tileData",
        sol::constructors<>(),
        "pressuredW" , sol::readonly(&TileData::pressuredW),
        "pressuredS" , sol::readonly(&TileData::pressuredS),
        "pressuredG" , sol::readonly(&TileData::pressuredG),
        "pressuredB" , sol::readonly(&TileData::pressuredB),
        "pressured"  , sol::readonly(&TileData::pressured),
        "powered"    , sol::readonly(&TileData::powered),
        "occupied"   , sol::readonly(&TileData::occupied),
        "terrain"    , sol::readonly(&TileData::terrain),
        "object"     , sol::readonly(&TileData::object),
        "player"     , sol::readonly(&TileData::player),
        "enemy"      , sol::readonly(&TileData::enemy),
        "playerid"   , sol::readonly(&TileData::playerId),
        "enemyid"    , sol::readonly(&TileData::enemyId),
        "directlyLit", sol::readonly(&TileData::directlyLit)
    );

    scripts[scri]->new_usertype<LevelStateData>
    (
        "levelData",
        sol::constructors<>(),
        "width"      , sol::readonly(&LevelStateData::width),
        "height"     , sol::readonly(&LevelStateData::height),
        "playercount", sol::readonly(&LevelStateData::playerCount),
        "playerx"    , sol::readonly(&LevelStateData::playerX),
        "playery"    , sol::readonly(&LevelStateData::playerY),
        "tiles"      , sol::readonly(&LevelStateData::tiles)
    );

    scripts[scri]->new_usertype<DamageInfo>
    (
        "damageInfo",
        sol::constructors<>(),
        "amount"       , sol::readonly(&DamageInfo::amount),
        "type"         , sol::readonly(&DamageInfo::type),
        "players"      , sol::readonly(&DamageInfo::players),
        "enemies"      , sol::readonly(&DamageInfo::enemies),
        "terrain"      , sol::readonly(&DamageInfo::terrain),
        "direction"    , sol::readonly(&DamageInfo::dir),
        "ground"       , sol::readonly(&DamageInfo::ground),
        "precise"      , sol::readonly(&DamageInfo::precise),
        "knockbackStr" , sol::readonly(&DamageInfo::knockbackStr),
        "statusEffects", sol::readonly(&DamageInfo::statusEffects)
    );

    scripts[scri]->new_usertype<LevelMessage>
    (
        "levelMessage",
        sol::constructors<LevelMessage()>(),
        "source"    , &LevelMessage::source,
        "message"   , &LevelMessage::message,
        "argKeys"   , &LevelMessage::argKeys,
        "argValsInt", &LevelMessage::argValsInt,
        "argValsStr", &LevelMessage::argValsStr
    );

    // Link object to apropriate script
    int linked = false;
    for (int i = 0; i < _OBJECT_SCRIPTS.size(); i++) {
        if (ID == _OBJECT_SCRIPTS[i].id) {
            // Open script file
            scripts[scri]->script_file(_OBJECT_SCRIPTS[i].filePath);

            // Set variant
            (*scripts[scri])["VARIANT"] = VR;

            // Set sprites
            for (int j = 0; j < _OBJECT_SCRIPTS[i].spriteIndexes.size(); j++) {
                (*scripts[scri])["spriteindexes"][j + 1] = _OBJECT_SCRIPTS[i].spriteIndexes[j];
            }

            linked = true;
            break;
        }
    }

    // Destroy object if no script is found
    if (!linked) {
        destroyed = true;
        return;
    }

    // Deserialize data
    if (data.length() > 0) {
        (*scripts[scri])["deserialize"](data);
    }

    // Set health variables
    maxHealth = (*scripts[scri])["maxhealth"];
    health = (*scripts[scri])["health"];
    armor = (*scripts[scri])["armor"];

    healthbarHeight = (*scripts[scri])["healthbarheight"];
    healthbarVisible = (*scripts[scri])["healthbarvisible"];

    // Set sprite variables
    currentSprite = EMPTY_SPRITE;
    sOffset = { -sprites[currentSprite].GetCenterX(), -sprites[currentSprite].GetCenterY() };
    sGround = (*scripts[scri])["ground"];

    // Set hitbox
    sol::optional<int> hType = (*scripts[scri])["hitbox"]["shape"];
    hitbox.type = hType.value();

    sol::optional<int> w = (*scripts[scri])["hitbox"]["size"]["w"];
    sol::optional<int> h = (*scripts[scri])["hitbox"]["size"]["h"];
    sol::optional<int> r = (*scripts[scri])["hitbox"]["size"]["r"];

    hitbox.width = w.value();
    hitbox.height = h.value();
    hitbox.radius = r.value();

    sol::optional<int> hox = (*scripts[scri])["hitbox"]["offset"]["x"];
    sol::optional<int> hoy = (*scripts[scri])["hitbox"]["offset"]["y"];

    hOffset = { hox.value(), hoy.value() };
}

void DynamicObject::update(std::vector<LevelMessage> &messages, const LevelStateData &ld, long int curtime)
{
    // Set variables
    (*scripts[scri])["moving"] = moveState.ongoing;
    (*scripts[scri])["moveprogress"] = moveState.progress;

    // Run logic
    (*scripts[scri])["update"](cPos, ld, maintime::currentGameTime / 1000);

    if (!moveState.ongoing) {
        // Check for a movement call
        if ((*scripts[scri])["startmovement"] == true) {
            if ((*scripts[scri])["movement"]["relative"] == true) {
                move_by_(
                    (*scripts[scri])["movement"]["x"],
                    (*scripts[scri])["movement"]["y"],
                    (*scripts[scri])["movement"]["duration"]
                );
            }
            else {
                move_to_(
                    (*scripts[scri])["movement"]["x"],
                    (*scripts[scri])["movement"]["y"],
                    (*scripts[scri])["movement"]["duration"]
                );
            }

            (*scripts[scri])["reset_movement"]();
        }
    }
    else {
        update_movement(ld);
    }

    // Get messages
    int count = (*scripts[scri])["messagecount"].get<int>();
    for (int i = 0; i < count; i++) {
        messages.push_back((*scripts[scri])["get_message"](i));
    }
    (*scripts[scri])["clear_messages"]();

    // Set grid position
    gPos = cPos / cellSize;

    // Set sprite
    currentSprite = (*scripts[scri])["select_sprite"](maintime::currentGameTime / 1000);
    sOffset = { -sprites[currentSprite].GetCenterX(), -sprites[currentSprite].GetCenterY() };
    sPos = cPos + sOffset;
    sPos.y -= height;
    sGround = (*scripts[scri])["ground"];

    // Set healthbar
    maxHealth = (*scripts[scri])["maxhealth"];
    health = (*scripts[scri])["health"];
    armor = (*scripts[scri])["armor"];

    healthbarHeight = (*scripts[scri])["healthbarheight"];
    healthbarVisible = (*scripts[scri])["healthbarvisible"];

    // Set hitbox
    hitbox.cPos = cPos;

    // Set other variables
    sol::optional<int> trs = (*scripts[scri])["transparent"];
    sol::optional<int> obs = (*scripts[scri])["obstructive"];
    sol::optional<int> pre = (*scripts[scri])["pressuring"];
    sol::optional<int> pwr = (*scripts[scri])["powering"];
    transparent = trs.value();
    obstructive = obs.value();
    pressuring = pre.value();
    powering = pwr.value();

    facing = static_cast<Direction>((*scripts[scri])["facing"]);

    return;
}

void DynamicObject::damage(std::vector<LevelMessage> &messages, const DamageInfo &di)
{
    destroyed = (*scripts[scri])["damage"](di);

    if (!moveState.ongoing) {
        // Check for a movement call
        if ((*scripts[scri])["startmovement"] == true) {
            if ((*scripts[scri])["movement"]["relative"] == true) {
                move_by_(
                    (*scripts[scri])["movement"]["x"],
                    (*scripts[scri])["movement"]["y"],
                    (*scripts[scri])["movement"]["duration"]
                );
            }
            else {
                move_to_(
                    (*scripts[scri])["movement"]["x"],
                    (*scripts[scri])["movement"]["y"],
                    (*scripts[scri])["movement"]["duration"]
                );
            }
        }
    }
}

void DynamicObject::destroy()
{
    scripts.ReleaseObj(scri);
}
