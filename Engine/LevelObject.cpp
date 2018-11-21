#include "LevelObject.h"

#include "GlobalData.h"
#include "Functions.h"

#include "LevelStateData.h"

#include <exception>

#include <fstream>

void LevelObject::move(Pos2D pos, int duration, int jumpHeight)
{
    if (pos == gPos) return;

    moveState.ongoing  = true;
    moveState.duration = duration;
    moveState.progress = 0.0f;
    moveState.start    = maintime::now();

    moveStartPos = gPos;
    moveEndPos   = pos;

    moveHeight = jumpHeight;

    if (duration != 0) {
        moveDirection = (pos - gPos).vdirection();
    }
}

StaticObject::StaticObject(int id, Pos2D gPos)
{
    // Set essential data
    this->oid = id;
    this->ID = oid / 1000;
    this->VR = oid % 1000;

    this->gPos = gPos;

    cPos = gPos * cellSize + Pos2D(cellSize / 2);

    // Set up script state
    script.open_libraries(sol::lib::base);

    script.new_usertype<Pos2D>
    (
        "position",
        sol::constructors<>(),
        "x", sol::readonly(&Pos2D::x),
        "y", sol::readonly(&Pos2D::y)
    );

    script.new_usertype<TileData>
    (
        "tileData",
        sol::constructors<>(),
        "pressuredW" , sol::readonly(&TileData::pressuredW ),
        "pressuredS" , sol::readonly(&TileData::pressuredS ),
        "pressuredG" , sol::readonly(&TileData::pressuredG ),
        "pressuredB" , sol::readonly(&TileData::pressuredB ),
        "pressured"  , sol::readonly(&TileData::pressured  ),
        "powered"    , sol::readonly(&TileData::powered    ),
        "terrain"    , sol::readonly(&TileData::terrain    ),
        "object"     , sol::readonly(&TileData::object     ),
        "player"     , sol::readonly(&TileData::player     ),
        "enemy"      , sol::readonly(&TileData::enemy      ),
        "directlyLit", sol::readonly(&TileData::directlyLit)
    );

    script.new_usertype<LevelStateData>
    (
        "levelData",
        sol::constructors<>(),
        "width" , sol::readonly(&LevelStateData::width ),
        "height", sol::readonly(&LevelStateData::height),
        "tiles" , sol::readonly(&LevelStateData::tiles )
    );

    // Link object to apropriate script
    int linked = false;
    for (int i = 0; i < _OBJECT_SCRIPTS.size(); i++) {
        if (id / 1000 == _OBJECT_SCRIPTS[i].id) {
            // Open script file
            script.script_file(_OBJECT_SCRIPTS[i].filePath);

            // Set sprites
            for (int j = 0; j < _OBJECT_SCRIPTS[i].spriteIndexes.size(); j++) {
                script["spriteindexes"][j] = _OBJECT_SCRIPTS[i].spriteIndexes[j];
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
    sol::optional<int> sox = script["spriteoffset"]["x"];
    sol::optional<int> soy = script["spriteoffset"]["y"];
    sOffset = { sox.value(), soy.value() };

    currentSprite = EMPTY_SPRITE;

    // Set hitbox
    sol::optional<int> hType = script["hitbox"]["type"];
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
    // Run logic
    script["update"](cPos, ld, curTime);

    // Set sprite
    sPos = cPos + sOffset;

    // Set hitbox
    hitbox.cPos = cPos;

    // Set other variables
    sol::optional<int> obs = script["obstructive"];
    sol::optional<int> pre = script["pressuring"];
    sol::optional<int> pwr = script["powering"];
    obstructive = obs.value();
    pressuring  = pre.value();
    powering    = pwr.value();

    return;
}
