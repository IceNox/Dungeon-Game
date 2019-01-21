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
            script["facing"] = (int)facing;
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

void LevelObject::update_movement()
{
    // Get time elapsed
    int mselapsed = (maintime::now() - moveState.start).get_duration(MILLISECONDS);
    moveState.progress = (double)mselapsed / moveState.duration;

    // End movement state if necessary
    if (moveState.progress >= 1.0f) {
        cPos = moveEndPos;
        height = 0;

        moveState.ongoing = false;
        script["reset_movement"]();

        return;
    }

    // Calculate position
    float movedBy = 0.0f;
    float mheight = 0.0f;

    // Default move path
    if (script["movement"]["mtype"] == -1) {
        movedBy = 1.0f - (1.0f - moveState.progress) * (1.0f - moveState.progress);

        float xAxis = 2 * (moveState.progress - 0.5f);
        float yAxis = xAxis * xAxis * -1.0f;

        mheight = yAxis * 30.0f + 30.0f;
    }
    // Script calculated move path
    else {
        sol::tie(movedBy, mheight) = script["movementtypes"][script["movement"]["mtype"]](moveState.progress);
    }

    Pos2D moveVec = moveEndPos - moveStartPos;

    cPos = moveStartPos + moveVec * movedBy;
    height = ceil(mheight);
}

StaticObject::StaticObject(const StaticObject &o)
    : StaticObject(o.oid, o.gPos)
{}

StaticObject& StaticObject::operator=(const StaticObject &o)
{
    return *this;
}

StaticObject::StaticObject(int id, Pos2D gPos, bool setup)
{
    if (!setup) return;

    // Set essential data
    this->oid = id;
    this->ID = oid / 1000;
    this->VR = oid % 1000;

    this->gPos = gPos;

    cPos = gPos * cellSize + Pos2D(cellSize / 2);

    // Set up script state
    script.open_libraries(sol::lib::base);
    script.open_libraries(sol::lib::math);

    script.new_usertype<Pos2D>
    (
        "position",
        sol::constructors<Pos2D()>(),
        "x", sol::readonly(&Pos2D::x),
        "y", sol::readonly(&Pos2D::y)
    );

    script.new_usertype<TileData>
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

    script.new_usertype<LevelStateData>
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

    script.new_usertype<DamageInfo>
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

    script.new_usertype<LevelMessage>
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
        if (id / 1000 == _OBJECT_SCRIPTS[i].id) {
            // Open script file
            script.script_file(_OBJECT_SCRIPTS[i].filePath);

            // Set sprites
            for (int j = 0; j < _OBJECT_SCRIPTS[i].spriteIndexes.size(); j++) {
                script["spriteindexes"][j+1] = _OBJECT_SCRIPTS[i].spriteIndexes[j];
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

    // Set sprite variables
    currentSprite = EMPTY_SPRITE;
    sOffset = { -sprites[currentSprite].GetCenterX(), -sprites[currentSprite].GetCenterY() };

    // Set hitbox
    sol::optional<int> hType = script["hitbox"]["shape"];
    hitbox.type = hType.value();

    sol::optional<int> w = script["hitbox"]["size"]["w"];
    sol::optional<int> h = script["hitbox"]["size"]["h"];
    sol::optional<int> r = script["hitbox"]["size"]["r"];

    hitbox.width  = w.value();
    hitbox.height = h.value();
    hitbox.radius = r.value();
    
    sol::optional<int> hox = script["hitbox"]["offset"]["x"];
    sol::optional<int> hoy = script["hitbox"]["offset"]["y"];

    hOffset = { hox.value(), hoy.value() };

    // Set light source
    sol::optional<int> isLS = script["lightsourcecount"];
    lightSource = isLS.value();

    for (int i = 0; i < isLS.value(); i++) {
        sol::optional<int> lsX = script["lightsources"][i]["offset"]["x"];
        sol::optional<int> lsY = script["lightsources"][i]["offset"]["y"];
        lightSourcePos.push_back(Pos2D(lsX.value(), lsY.value()));

        sol::optional<int> str = script["lightsources"][i]["strength"];
    }
}

void StaticObject::update(const LevelStateData &ld, long int curTime)
{
    // Set variables
    script["moving"] = moveState.ongoing;
    script["moveprogress"] = moveState.progress;

    // Run logic
    script["update"](cPos, ld, maintime::currentGameTime / 1000);

    if (!moveState.ongoing) {
        // Check for a movement call
        if (script["startmovement"] == true) {
            if (script["movement"]["relative"] == true) {
                move_by_(
                    script["movement"]["x"],
                    script["movement"]["y"],
                    script["movement"]["duration"]
                );
            }
            else {
                move_to_(
                    script["movement"]["x"],
                    script["movement"]["y"],
                    script["movement"]["duration"]
                );
            }
        }
    }
    else {
        update_movement();
    }

    // Set grid position
    gPos = cPos / cellSize;

    // Set sprite
    currentSprite = script["select_sprite"](maintime::currentGameTime / 1000);
    sOffset = { -sprites[currentSprite].GetCenterX(), -sprites[currentSprite].GetCenterY() };
    sPos = cPos + sOffset;
    sPos.y -= height;

    // Set hitbox
    hitbox.cPos = cPos;

    // Set other variables
    sol::optional<int> obs = script["obstructive"];
    sol::optional<int> pre = script["pressuring"];
    sol::optional<int> pwr = script["powering"];
    obstructive = obs.value();
    pressuring  = pre.value();
    powering    = pwr.value();

    facing = static_cast<Direction>(script["facing"]);

    return;
}

void StaticObject::damage(const DamageInfo &di)
{
    destroyed = script["damage"](di, destroyed);

    if (!moveState.ongoing) {
        // Check for a movement call
        if (script["startmovement"] == true) {
            if (script["movement"]["relative"] == true) {
                move_by_(
                    script["movement"]["x"],
                    script["movement"]["y"],
                    script["movement"]["duration"]
                );
            }
            else {
                move_to_(
                    script["movement"]["x"],
                    script["movement"]["y"],
                    script["movement"]["duration"]
                );
            }
        }
    }
}

DynamicObject::DynamicObject(const DynamicObject &o)
    : DynamicObject(o.oid, o.gPos)
{}

DynamicObject& DynamicObject::operator=(const DynamicObject &o)
{
    return *this;
}

DynamicObject::DynamicObject(int id, Pos2D gPos)
{
    // Set essential data
    this->oid = id;
    this->ID = oid / 1000;
    this->VR = oid % 1000;

    this->gPos = gPos;

    cPos = gPos * cellSize + Pos2D(cellSize / 2);

    // Set up script state
    script.open_libraries(sol::lib::base);
    script.open_libraries(sol::lib::math);

    script.new_usertype<Pos2D>
    (
        "position",
        sol::constructors<Pos2D()>(),
        "x", &Pos2D::x,
        "y", &Pos2D::y
    );

    script.new_usertype<TileData>
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

    script.new_usertype<LevelStateData>
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

    script.new_usertype<LevelMessage>
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
        if (id / 1000 == _OBJECT_SCRIPTS[i].id) {
            // Open script file
            script.script_file(_OBJECT_SCRIPTS[i].filePath);

            // Set sprites
            for (int j = 0; j < _OBJECT_SCRIPTS[i].spriteIndexes.size(); j++) {
                script["spriteindexes"][j + 1] = _OBJECT_SCRIPTS[i].spriteIndexes[j];
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

    // Set health variables
    maxHealth = script["maxhealth"];
    health = script["health"];
    armor = script["armor"];

    healthbarHeight = script["healthbarheight"];
    healthbarVisible = false;

    // Set sprite variables
    currentSprite = EMPTY_SPRITE;
    sOffset = { -sprites[currentSprite].GetCenterX(), -sprites[currentSprite].GetCenterY() };

    // Set hitbox
    sol::optional<int> hType = script["hitbox"]["shape"];
    hitbox.type = hType.value();

    sol::optional<int> w = script["hitbox"]["size"]["w"];
    sol::optional<int> h = script["hitbox"]["size"]["h"];
    sol::optional<int> r = script["hitbox"]["size"]["r"];

    hitbox.width = w.value();
    hitbox.height = h.value();
    hitbox.radius = r.value();

    sol::optional<int> hox = script["hitbox"]["offset"]["x"];
    sol::optional<int> hoy = script["hitbox"]["offset"]["y"];

    hOffset = { hox.value(), hoy.value() };
}

void DynamicObject::update(std::vector<LevelMessage> &messages, const LevelStateData &ld, long int curtime)
{
    // Set variables
    script["moving"] = moveState.ongoing;
    script["moveprogress"] = moveState.progress;

    // Run logic
    script["update"](cPos, ld, maintime::currentGameTime / 1000);

    if (!moveState.ongoing) {
        // Check for a movement call
        if (script["startmovement"] == true) {
            if (script["movement"]["relative"] == true) {
                move_by_(
                    script["movement"]["x"],
                    script["movement"]["y"],
                    script["movement"]["duration"]
                );
            }
            else {
                move_to_(
                    script["movement"]["x"],
                    script["movement"]["y"],
                    script["movement"]["duration"]
                );
            }

            script["reset_movement"]();
        }
    }
    else {
        update_movement();
    }

    // Get messages
    int count = script["messagecount"].get<int>();
    for (int i = 0; i < count; i++) {
        messages.push_back(script["get_message"](i));
    }
    script["clear_messages"]();

    // Set grid position
    gPos = cPos / cellSize;

    // Set sprite
    currentSprite = script["select_sprite"](maintime::currentGameTime / 1000);
    sOffset = { -sprites[currentSprite].GetCenterX(), -sprites[currentSprite].GetCenterY() };
    sPos = cPos + sOffset;
    sPos.y -= height;

    // Set hitbox
    hitbox.cPos = cPos;

    // Set other variables
    sol::optional<int> obs = script["obstructive"];
    sol::optional<int> pre = script["pressuring"];
    sol::optional<int> pwr = script["powering"];
    obstructive = obs.value();
    pressuring = pre.value();
    powering = pwr.value();

    facing = static_cast<Direction>(script["facing"]);

    return;
}

void DynamicObject::damage(std::vector<LevelMessage> &messages, const DamageInfo &di)
{
    destroyed = script["damage"](di);

    if (!moveState.ongoing) {
        // Check for a movement call
        if (script["startmovement"] == true) {
            if (script["movement"]["relative"] == true) {
                move_by_(
                    script["movement"]["x"],
                    script["movement"]["y"],
                    script["movement"]["duration"]
                );
            }
            else {
                move_to_(
                    script["movement"]["x"],
                    script["movement"]["y"],
                    script["movement"]["duration"]
                );
            }
        }
    }
}
