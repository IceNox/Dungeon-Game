#include "Level.h"

#include "GlobalData.h"

#include "Functions.h"

#include <sstream>

int _LEVEL_WIDTH;
int _LEVEL_HEIGHT;

std::vector<Pos2D> _PLAYER_GRID_POS = { Pos2D(0, 0), Pos2D(0, 0) };

int playerGridX = 1;
int playerGridY = 1;

int levelWidth;
int levelHeight;

Level::Level() {}

void Level::load_game(LevelType lType, std::string lName)
{
    if      (lType == NEW_GAME) {
        // TODO New game
    }
    else if (lType == SAVE) {
        load_save_file(lName + ".json");
    }
    else if (lType == USER) {
        load_level(lName + ".json");
    }
}

void Level::load_save_file(std::string saveName)
{
    saveData.read_from_file(saveName);

    // Clear all level data
    messages.clear();

    players.clear();
    portals.clear();
    tiles.clear();
    wires.clear();
    levelObjects.clear();

    crates.clear();
    pressurePlates.clear();
    lamps.clear();
    doors.clear();
    signs.clear();

    barbarians.clear();
    wizards.clear();
    paladins.clear();
    zombies.clear();
    liches.clear();
    wraiths.clear();

    entities.clear();
    animations.clear();
    particles.clear();

    commands.clear();
    damageMap.clear();
    healMap.clear();

    // Set initial variables
    currentStage = saveData.currentStage;
    currentLevel = saveData.currentLevel;

    sl_index = currentStage * levelsInStage + currentLevel;

    width  = saveData.levelData[sl_index].width;
    height = saveData.levelData[sl_index].height;
    _LEVEL_WIDTH  = width;
    _LEVEL_HEIGHT = height;
    reachedExit = false;
    playerDied  = false;

    inDeathScreen      = false;
    startedDeathEffect = false;
    inEndScreen        = false;
    startedEndEffect   = false;

    // Set tile data grid
    lsd::set(levelStateData, width, height);

    // Spawn players
    for (int i = 0; i < saveData.playerCount; i++) {
        // Create player objects
        players.push_back(Player(saveData.levelData[sl_index].playerStartPos));

        // Set player stats
        players[i].health = saveData.playerHealth[i];
        players[i].armor  = saveData.playerArmor[i];
        players[i].gold   = saveData.playerGold[i];

        // TODO Fill player inventory
        /*
        for (int j = 0; j < 5; j++) {
            players[i].items[j] = saveData.playerInventory[i].playerItems[j];
        }
        */
    }

    // Read level data
    baseFloorVariant   = saveData.levelData[sl_index].baseFloorVariant;
    tileCount          = width * height;
    wireCount          = width * height;
    portalCount        = saveData.levelData[sl_index].portalCount;

    /*
    wireCount          = saveData.levelData[sl_index].wireCount;
    crateCount         = saveData.levelData[sl_index].crateCount;
    pressurePlateCount = saveData.levelData[sl_index].pressurePlateCount;
    lampCount          = saveData.levelData[sl_index].lampCount;
    doorCount          = saveData.levelData[sl_index].doorCount;
    signCount          = saveData.levelData[sl_index].signCount;
    shooterCount       = saveData.levelData[sl_index].shooterCount;
    barbarianCount     = saveData.levelData[sl_index].barbarianCount;
    wizardCount        = saveData.levelData[sl_index].wizardCount;
    paladinCount       = saveData.levelData[sl_index].paladinCount;
    zombieCount        = saveData.levelData[sl_index].zombieCount;
    lichCount          = saveData.levelData[sl_index].lichCount;
    wraithCount        = saveData.levelData[sl_index].wraithCount;
    */

    sObjectCount       = saveData.levelData[sl_index].sObjectCount;
    dObjectCount       = saveData.levelData[sl_index].dObjectCount;

    crateCount         = 0;
    pressurePlateCount = 0;
    lampCount          = 0;
    doorCount          = 0;
    signCount          = 0;
    barbarianCount     = 0;
    wizardCount        = 0;
    paladinCount       = 0;
    zombieCount        = 0;
    lichCount          = 0;
    wraithCount        = 0;

    // Create minimap
    minimap.create_minimap(width, height);

    // Fill wire and tile vectors
    for (int i = 0; i < tileCount; i++) {
        wires.push_back(Wire());
        tiles.push_back(Tile());
    }

    // Fill wires
    for (int y = 0; y < _LEVEL_HEIGHT; y++) {
        for (int x = 0; x < _LEVEL_WIDTH; x++) {
            wires[y * _LEVEL_WIDTH + x].set_wire(Pos2D(x, y));
        }
    }

    // Fill edge walls
    for (int y = 0; y < _LEVEL_HEIGHT; y++) {
        for (int x = 0; x < _LEVEL_WIDTH; x++) {
            if (y == 0 || y == height - 1 || x == 0 || x == width - 1) {
                tiles[y * _LEVEL_WIDTH + x].set_tile(Pos2D(x, y), WALL, V_STONE, false, false, true);
            }
        }
    }

    // Fill inner tiles
    for (int i = 0; i < saveData.levelData[sl_index].wallCount; i++) {
        int index = saveData.levelData[sl_index].wallGridPos[i].index(_LEVEL_WIDTH);

        tiles[index].set_tile
        (
            saveData.levelData[sl_index].wallGridPos[i],
            WALL,
            saveData.levelData[sl_index].wallVariant[i],
            saveData.levelData[sl_index].wallActivatable[i],
            saveData.levelData[sl_index].wallToggleable[i],
            saveData.levelData[sl_index].wallActive[i]
        );
    }

    for (int i = 0; i < saveData.levelData[sl_index].floorCount; i++) {
        int index = saveData.levelData[sl_index].wallGridPos[i].index(_LEVEL_WIDTH);
        
        tiles[index].set_tile
        (
            saveData.levelData[sl_index].floorGridPos[i],
            FLOOR,
            saveData.levelData[sl_index].floorVariant[i],
            false,
            false,
            false
        );
    }

    /*
    for (int j = 0; j < levelInfo[i].lavaCount; j++) {
    if (levelInfo[i].lavaX[j] == x && levelInfo[i].lavaY[j] == y) {
    std::string spriteNames[3] = { "lava_1", "lava_2", "lava_3" };
    tiles.push_back(Tile(x, y, "lava", "lava", false, false, false, 3, spriteNames, 0.5f));
    lavaPlaced = true;
    }
    }

    for (int j = 0; j < levelInfo[i].waterCount; j++) {
        if (levelInfo[i].waterX[j] == x && levelInfo[i].waterY[j] == y) {
            std::string spriteNames[3] = { "water_1", "water_2", "water_3" };
            tiles.push_back(Tile(x, y, "water", "water", false, false, false, 3, spriteNames, 0.25f));
            waterPlaced = true;
        }
    }
    */

    // Fill remaining base floor tiles
    for (int y = 0; y < _LEVEL_HEIGHT; y++) {
        for (int x = 0; x < _LEVEL_WIDTH; x++) {
            if (!tiles[y * _LEVEL_WIDTH + x].isSet) {
                tiles[y * _LEVEL_WIDTH + x].set_tile(Pos2D(x, y), BASE_FLOOR, saveData.levelData[sl_index].baseFloorVariant, false, false, false);
            }
        }
    }

    // Fill healthMap array
    for (int i = 0; i < tileCount; i++) {
        healthMap.push_back(HealthMap());
    }

    // Read portals
    for (int i = 0; i < portalCount; i++) {
        portals.push_back(
            Portal(
                saveData.levelData[sl_index].portalPos[i],
                saveData.levelData[sl_index].portalExitPos[i],
                saveData.levelData[sl_index].portalNewLevel[i]
            )
        );
    }

    // Read static objects
    for (int i = 0; i < sObjectCount; i++) {
        levelObjects.push_back(
            new StaticObject(
                saveData.levelData[sl_index].sObjectID[i],
                saveData.levelData[sl_index].sObjectPos[i]
            )
        );
    }

    levelStartTime = maintime::now();

    inFA    = true;
    FAstart = maintime::now();
}

/*
void Level::load_level(std::string levelName, std::vector<LevelData> &levelData) 
{
    using namespace std::chrono;

    for (int i = 0; i < levelInfo.size(); i++) {
        if (levelInfo[i].levelName == levelName) {
            // Clear all level data
            player.clear();
            tiles = nullptr;
            wires = nullptr;

            crates.clear();
            pressurePlates.clear();
            lamps.clear();
            doors.clear();
            signs.clear();

            shooters.clear();
            barbarians.clear();
            wizards.clear();
            paladins.clear();
            zombies.clear();
            liches.clear();
            wraiths.clear();

            entities.clear();
            visualEffects.clear();
            particles.clear();

            commands.clear();

            damageMap.clear();
            healMap.clear();

            // Set initial variables
            width        = levelInfo[i].width;
            height        = levelInfo[i].height;
            levelWidth    = width;
            levelHeight = height;
            reachedExit = false;
            playerDied    = false;

            inDeathScreen        = false;
            startedDeathEffect    = false;
            inEndScreen         = false;
            startedEndEffect = false;
            
            // Create player
            player.push_back(Player(levelInfo[i].playerStartX, levelInfo[i].playerStartY));

            // Read level data
            exit.gX          = levelInfo[i].exitX;
            exit.gY          = levelInfo[i].exitY;
            baseFloorType = levelInfo[i].baseFloorType;

            tileCount           = width * height;
            wireCount           = levelInfo[i].wireCount;
            crateCount           = levelInfo[i].crateCount;
            pressurePlateCount = levelInfo[i].pressurePlateCount;
            lampCount           = levelInfo[i].lampCount;
            doorCount           = levelInfo[i].doorCount;
            signCount           = levelInfo[i].signCount;
            shooterCount       = levelInfo[i].shooterCount;
            barbarianCount       = levelInfo[i].barbarianCount;
            wizardCount           = levelInfo[i].wizardCount;
            paladinCount       = levelInfo[i].paladinCount;
            zombieCount           = levelInfo[i].zombieCount;
            lichCount           = levelInfo[i].lichCount;
            wraithCount           = levelInfo[i].wraithCount;

            // Create minimap
            minimap.create_minimap(width, height);

            // Fill wire array
            wires = new Wire[wireCount];
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    wires.push_back(Wire(x, y));
                }
            }

            // Fill yilee array
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    if (y == 0 || y == height - 1 || x == 0 || x == width - 1) {
                        tiles.push_back(Tile(x, y, "wall", "stone", true, false, false));
                    }
                    else {
                        bool wallPlaced     = false;
                        bool floorPlaced = false;
                        bool lavaPlaced  = false;
                        bool waterPlaced = false;

                        for (int j = 0; j < levelInfo[i].wallCount; j++) {
                            if (levelInfo[i].wallX[j] == x && levelInfo[i].wallY[j] == y) {
                                tiles.push_back(Tile(x, y, "wall", levelInfo[i].wallType[j], true, levelInfo[i].wallActivatable[j], levelInfo[i].wallToggleable[j]));
                                wallPlaced = true;
                            }
                        }
                        for (int j = 0; j < levelInfo[i].floorCount; j++) {
                            if (levelInfo[i].floorX[j] == x && levelInfo[i].floorY[j] == y) {
                                tiles.push_back(Tile(x, y, "floor", levelInfo[i].floorType[j], false, false, false));
                                floorPlaced = true;
                            }
                        }
                        for (int j = 0; j < levelInfo[i].lavaCount; j++) {
                            if (levelInfo[i].lavaX[j] == x && levelInfo[i].lavaY[j] == y) {
                                std::string spriteNames[3] = { "lava_1", "lava_2", "lava_3" };
                                tiles.push_back(Tile(x, y, "lava", "lava", false, false, false, 3, spriteNames, 0.5f));
                                lavaPlaced = true;
                            }
                        }
                        for (int j = 0; j < levelInfo[i].waterCount; j++) {
                            if (levelInfo[i].waterX[j] == x && levelInfo[i].waterY[j] == y) {
                                std::string spriteNames[3] = { "water_1", "water_2", "water_3" };
                                tiles.push_back(Tile(x, y, "water", "water", false, false, false, 3, spriteNames, 0.25f));
                                waterPlaced = true;
                            }
                        }
                        if (!wallPlaced && !floorPlaced && !lavaPlaced && !waterPlaced) {
                            tiles.push_back(Tile(x, y, "base_floor", levelInfo[i].baseFloorType, false, false, false));
                        }
                    }
                }
            }

            for (int j = 0; j < wireCount; j++) {
                int index = levelInfo[i].wireY[j] * width + levelInfo[i].wireX[j];
                wires[index].gX          = levelInfo[i].wireX[j];
                wires[index].gY          = levelInfo[i].wireY[j];
                wires[index].gate      = levelInfo[i].wireGate[j];
                wires[index].gateType = levelInfo[i].wireGateType[j];
                wires[index].facing   = levelInfo[i].wireFacing[j];

                if        (levelInfo[i].wireColor[j] == "red") {
                    wires[index].red = true;
                }
                else if (levelInfo[i].wireColor[j] == "blue") {
                    wires[index].blue = true;
                }
                else if (levelInfo[i].wireColor[j] == "green") {
                    wires[index].green = true;
                }
                else if (levelInfo[i].wireColor[j] == "yellow") {
                    wires[index].yellow = true;
                }
            }

            for (int j = 0; j < crateCount; j++) {
                crates.push_back(
                    Crate(
                        levelInfo[i].crateX[j],
                        levelInfo[i].crateY[j]
                    )
                );
            }

            for (int j = 0; j < pressurePlateCount; j++) {
                pressurePlates.push_back(
                    PressurePlate(
                        levelInfo[i].plateX[j],
                        levelInfo[i].plateY[j],
                        levelInfo[i].plateType[j]
                    )
                );
            }

            for (int j = 0; j < lampCount; j++) {
                lamps.push_back(
                    Lamp(
                        levelInfo[i].lampX[j],
                        levelInfo[i].lampY[j],
                        levelInfo[i].lampType[j],
                        levelInfo[i].lampToggleable[j]
                    )
                );
            }

            for (int j = 0; j < doorCount; j++) {
                doors.push_back(
                    Door(
                        levelInfo[i].doorX[j],
                        levelInfo[i].doorY[j],
                        levelInfo[i].doorFacing[j]
                    )
                );

                tiles[doors[j].gY * width + doors[j].gX].hasDoor = true;
            }

            for (int j = 0; j < signCount; j++) {
                signs.push_back(
                    Sign(
                        levelInfo[i].signX[j],
                        levelInfo[i].signY[j],
                        levelInfo[i].signText[j]
                    )
                );
            }

            for (int j = 0; j < shooterCount; j++) {
                shooters.push_back(
                    Shooter(
                        levelInfo[i].shooterX[j],
                        levelInfo[i].shooterY[j],
                        levelInfo[i].shooterType[j],
                        levelInfo[i].shooterOffset[j],
                        levelInfo[i].shooterFacing[j],
                        levelInfo[i].shooterActive[j]
                    )
                );
            }

            for (int j = 0; j < barbarianCount; j++) {
                barbarians.push_back(
                    Barbarian(
                        levelInfo[i].barbarianX[j],
                        levelInfo[i].barbarianY[j],
                        levelInfo[i].barbarianType[j],
                        commands
                    )
                );
            }
            
            for (int j = 0; j < wizardCount; j++) {
                wizards.push_back(
                    Wizard(
                        levelInfo[i].wizardX[j],
                        levelInfo[i].wizardY[j],
                        levelInfo[i].wizardType[j],
                        commands
                    )
                );
            }
            
            for (int j = 0; j < paladinCount; j++) {
                paladins.push_back(
                    Paladin(
                        levelInfo[i].paladinX[j],
                        levelInfo[i].paladinY[j],
                        levelInfo[i].paladinType[j],
                        commands
                    )
                );
            }

            for (int j = 0; j < zombieCount; j++) {
                zombies.push_back(
                    Zombie(
                        levelInfo[i].zombieX[j],
                        levelInfo[i].zombieY[j],
                        levelInfo[i].zombieType[j],
                        commands
                    )
                );
            }

            for (int j = 0; j < lichCount; j++) {
                liches.push_back(
                    Lich(
                        levelInfo[i].lichX[j],
                        levelInfo[i].lichY[j],
                        levelInfo[i].lichType[j],
                        commands
                    )
                );
            }

            for (int j = 0; j < wraithCount; j++) {
                wraiths.push_back(
                    Wraith(
                        levelInfo[i].wraithX[j],
                        levelInfo[i].wraithY[j],
                        levelInfo[i].wraithType[j],
                        commands
                    )
                );
            }
            
            for (int j = 0; j < tileCount; j++) {
                damageMap.push_back(
                    DamageMap(
                    )
                );
                healMap.push_back(
                    HealMap(
                    )
                );
            }

            break;
        }
    }

    levelStartTime = system_clock::now();

    inFA    = true;
    FAstart = system_clock::now();
}
*/

void Level::update_level(std::vector<GameMessage*> &msg, ScreenAnimations &screenAnimations, Key &k, UserData &userData)
{
    if (inFA) {
        //duration<float> elapsed_seconds = system_clock::now() - FAstart;
        //float FAtimeElapsed = elapsed_seconds.count();
        FAprogress = static_cast<float>((maintime::now() - FAstart).get_duration(MILLISECONDS) / FAlength);

        if (FAprogress >= 1.0f) inFA = false;
        if (FAprogress <  0.0f) FAprogress = 0.0f;
    }
    if (inEDFA) {
        //duration<float> elapsed_seconds = system_clock::now() - EDFAstart;
        //float EDFAtimeElapsed = elapsed_seconds.count();
        EDFAprogress = static_cast<float>((maintime::now() - EDFAstart).get_duration(MILLISECONDS) / EDFAlength);

        if (EDFAprogress < 0.0f) EDFAprogress = 0.0f;
        if (EDFAprogress >= 1.0f) {
            // TODO End of level fade
        }
    }

    if (inDeathScreen || inEndScreen) return;

    // Pause/unpause 
    if (k == userData.keyBindings.PAUSE) toggle_pause();

    // Open/close console (~)
    if (k == Key(VK_OEM_3, CLICKED)) {
        if (!console.is_opened()) {
            console.open();
        }
        else {
            console.close();
        }
    }

    // Update console
    console.update_console(k, messages);

    if (paused) return;

    // Set/Reset variables
    players[0].tookDamage   = false;
    players[0].damageAmount = 0;

    _PLAYER_GRID_POS[0] = players[0].gPos;
    
    visionCenterPos = players[0].cPos;
    if (players.size() == 2) {
        // if there are 2 players, set the vision center between them
        visionCenterPos = (players[0].cPos + players[1].cPos) / 2;
    }

    // Set tile data grid
    set_game_state_data();

    // Update all objects
    players[0].update_player
    (
        tiles,
        damageMap,
        commands,
        messages,
        screenAnimations,
        levelStateData,
        k,
        userData,
        console.is_opened()?true:false
    );

    for (int i = 0; i < _LEVEL_WIDTH * _LEVEL_HEIGHT; i++) {
        tiles[i].update_tile(wires, baseFloorVariant, damageMap);
    }
    for (auto it : levelObjects) {
        it->update(levelStateData, maintime::currentGameTime);
    }
    for (auto it : entities) {
        it->update();
    }
    for (unsigned i = 0; i < animations.size(); i++) {
        animations[i].update_animation();
    }
    for (unsigned i = 0; i < particles.size(); i++) {
        particles[i].update_particle();
    }

    for (unsigned i = 0; i < portals.size(); i++) {
        //portals[i].update_portal();
    }
    for (unsigned i = 0; i < crates.size(); i++) {
        //crates[i].update_crate();
    }
    for (unsigned i = 0; i < pressurePlates.size(); i++) {
        //pressurePlates[i].update_plate(tiles);
    }
    for (unsigned i = 0; i < lamps.size(); i++) {
        //lamps[i].update_lamp(wires);
    }
    for (unsigned i = 0; i < signs.size(); i++) {
        //signs[i].update_sign(player[0]);
    }

    // Mobs
    for (unsigned i = 0; i < barbarians.size(); i++) {
        //barbarians[i].update_barbarian(player[0], tiles, damageMap, commands);
    }
    for (unsigned i = 0; i < wizards.size(); i++) {
        //wizards[i].update_wizard(player[0], tiles, damageMap, commands);
    }
    for (unsigned i = 0; i < paladins.size(); i++) {
        //paladins[i].update_paladin(player[0], tiles, damageMap, commands);
    }
    for (unsigned i = 0; i < zombies.size(); i++) {
        //zombies[i].update_zombie(player[0], tiles, damageMap, commands, zombies);
    }
    for (unsigned i = 0; i < liches.size(); i++) {
        //liches[i].update_lich(player[0], tiles, damageMap, commands);
    }
    for (unsigned i = 0; i < wraiths.size(); i++) {
        //wraiths[i].update_wraith(player[0], tiles, commands);
    }
    for (unsigned i = 0; i < entities.size(); i++) {
        //entities[i].update_entity();
    }

    // Update level
    handle_messages();
    
    collide_entities(screenAnimations);
    deal_damage(screenAnimations);
    
    remove_destroyed_objects();
    remove_finished_animations();
    remove_finished_particles();
    
    merge_gold_on_ground();
    
    update_tile_info();
    update_wires();
    update_lighting();
    update_minimap(k, userData);

    finish_level();
}

void Level::set_game_state_data()
{
    lsd::reset(levelStateData);

    for (unsigned i = 0; i < tiles.size(); i++) {
        // Wire stuff
        levelStateData.tiles[i].pressuredW = tiles[i].pressuredW;
        levelStateData.tiles[i].pressuredS = tiles[i].pressuredS;
        levelStateData.tiles[i].pressuredG = tiles[i].pressuredG;
        levelStateData.tiles[i].pressuredB = tiles[i].pressuredB;
        levelStateData.tiles[i].pressured  = tiles[i].pressured;
        levelStateData.tiles[i].powered    = tiles[i].powered;

        // Occupation
        levelStateData.tiles[i].terrain = (tiles[i].type == WALL && tiles[i].active);

        // Lighting
        levelStateData.tiles[i].directlyLit = tiles[i].directlyLit;
    }

    for (unsigned i = 0; i < levelObjects.size(); i++) {
        // Set object occupation
        if (levelObjects[i]->obstructive) {
            int index = levelObjects[i]->gPos.index(_LEVEL_WIDTH);

            levelStateData.tiles[index].object = true;
        }

        // Set enemies
        if (levelObjects[i]->get_type() == OBJECT_DYNAMIC) {
            int index = levelObjects[i]->gPos.index(_LEVEL_WIDTH);

            levelStateData.tiles[index].enemy = true;
        }
    }

    for (unsigned i = 0; i < players.size(); i++) {
        // Set players
        int index = players[i].gPos.index(_LEVEL_WIDTH);

        levelStateData.tiles[index].player = true;
    }
}

void Level::handle_messages()
{
    for (unsigned i = 0; i < messages.size(); i++) {
        std::string source  = messages[i].source;
        std::string command = exer_str_until(messages[i].message);

        // Damage messages
        if (command == "damage" || command == "dmg") {
            std::string xstr = exer_str_until(messages[i].message);
            std::string ystr = exer_str_until(messages[i].message);

            int x = str_to_int(xstr);
            int y = str_to_int(ystr);

            healthMap[y * width + x].add_damage(messages[i]);
        }

        // Heal messages
        if (command == "heal") {
            std::string xstr = exer_str_until(messages[i].message);
            std::string ystr = exer_str_until(messages[i].message);

            int x = str_to_int(xstr);
            int y = str_to_int(ystr);

            healthMap[y * width + x].add_damage(messages[i]);
        }

        // Damage projectile messages
        if (command == "damageprojectile" || command == "dmgprojectile") {
            std::string result;
            entities.push_back(new DamageProjectile(messages[i], result));

            if (result.length() > 2) {
                entities.pop_back();
            }
        }

        // Heal projectile messages
        if (command == "healprojectile") {
            std::string result;
            entities.push_back(new HealProjectile(messages[i], result));

            if (result.length() > 2) {
                entities.pop_back();
            }
        }

        // Gold entity messages
        if (command == "gold") {
            std::string result;
            entities.push_back(new GoldEntity(messages[i], result));

            if (result.length() > 2) {
                entities.pop_back();
            }
        }

        // Animation messages
        if (command == "animation") {
            std::string result;
            animations.push_back(Animation(messages[i], result));

            if (result.length() > 2) {
                animations.pop_back();
            }
        }

        // Particle messages
        if (command == "particle") {
            std::string result;
            particles.push_back(Particle(messages[i], result));

            if (result.length() > 2) {
                particles.pop_back();
            }
        }
    }
    /*
    for (auto it : levelMessages) {
        it->handle_msg
        (
            particles,
            animations,
            entities
        );
    }
    */

    /*
    for (int i = 0; i < commands.size(); i++) {
        // Execute spawn commands
        if (commands[i].spawn) {
            if        (commands[i].mobName == "barbarian") barbarians    .push_back(Barbarian(commands[i].mobX, commands[i].mobY, commands[i].mobType, commands, commands[i].mobAnimated));
            else if (commands[i].mobName == "wizard")     wizards    .push_back(Wizard    (commands[i].mobX, commands[i].mobY, commands[i].mobType, commands, commands[i].mobAnimated));
            else if (commands[i].mobName == "paladin")     paladins    .push_back(Paladin    (commands[i].mobX, commands[i].mobY, commands[i].mobType, commands, commands[i].mobAnimated));
            else if (commands[i].mobName == "zombie")     zombies    .push_back(Zombie    (commands[i].mobX, commands[i].mobY, commands[i].mobType, commands, commands[i].mobAnimated));
            else if (commands[i].mobName == "lich")         liches        .push_back(Lich        (commands[i].mobX, commands[i].mobY, commands[i].mobType, commands, commands[i].mobAnimated));
            else if (commands[i].mobName == "wraith")     wraiths    .push_back(Wraith    (commands[i].mobX, commands[i].mobY, commands[i].mobType, commands, commands[i].mobAnimated));

            continue;
        }

        // Execute damage commands
        else if (commands[i].damageInRadius) {
            // SoonTm

            continue;
        }

        // Execute heal commands
        else if (commands[i].healInRadius) {
            // SoonTm

            continue;
        }

        // Execute entity creation commands
        else if (commands[i].createEntity) {
            if (commands[i].entityType == "regular") {
                entities.push_back(
                    Entity(
                        commands[i].entitygX,
                        commands[i].entitygY,
                        commands[i].entitytX,
                        commands[i].entitytY,
                        commands[i].entityDamage,
                        commands[i].entityHeal,
                        commands[i].entityHitboxRadius,
                        commands[i].entitySpriteoX,
                        commands[i].entitySpriteoY,
                        commands[i].entityCollidesWithPlayer,
                        commands[i].entityCollidesWithAI,
                        commands[i].entityvX,
                        commands[i].entityvY,
                        commands[i].entityRangeX,
                        commands[i].entityRangeY,
                        commands[i].entityAnimated,
                        commands[i].entityBaseSpriteName,
                        commands[i].entitySpriteW,
                        commands[i].entitySpriteH,
                        commands[i].entityFrameCount,
                        commands[i].entityFrameHoldTime
                    )
                );
            }
            else if (commands[i].entityType == "gold") {
                entities.push_back(
                    Entity(
                        commands[i].entitygX,
                        commands[i].entitygY,
                        commands[i].goldAmount
                    )
                );
            }

            else if (commands[i].entityType == "item") {
                entities.push_back(
                    Entity(
                        commands[i].entitygX,
                        commands[i].entitygY,
                        commands[i].itemName,
                        commands[i].itemSpriteName
                    )
                );
            }

            continue;
        }

        // Execute particle creation commands
        else if (commands[i].createParticle) {
            if (commands[i].particleType == "regular") {
                particles.push_back(
                    Particle(
                        commands[i].particlecX,
                        commands[i].particlecY,
                        commands[i].particleSprite,
                        commands[i].particlevX,
                        commands[i].particlevY,
                        commands[i].particleaX,
                        commands[i].particleaY,
                        commands[i].particleExistenceLength,
                        commands[i].particleFadeLength
                    )
                );
            }
            else if (commands[i].particleType == "text") {
                particles.push_back(
                    Particle(
                        commands[i].particlecX,
                        commands[i].particlecY,
                        commands[i].particleText
                    )
                );
            }

            continue;
        }

        // Execute visual effect display commands
        else if (commands[i].showVisualEffect) {
            visualEffects.push_back(
                VisualEffect(
                    commands[i].visualEffectsX,
                    commands[i].visualEffectsY,
                    commands[i].visualEffectGround,
                    commands[i].visualEffectName,
                    commands[i].visualEffectSpriteW,
                    commands[i].visualEffectSpriteH,
                    commands[i].visualEffectHoldTime,
                    commands[i].visualEffectFrameCount,
                    commands[i].visualEffectTransparency
                )
            );

            continue;
        }
    }
    */
    commands.clear();
}

void Level::collide_entities(ScreenAnimations &screenAnimations)
{
    { // Collide terrain
        for (int it = 0; it < entities.size(); it++) {
            int index = entities[it]->gPos.index(width);

            // Check if entity is in a wall/out of map
            if (tiles[index].type == WALL && tiles[index].active) {
                entities.erase(entities.begin() + it);
                it--;
                continue;
            }
            if (index < 0 || index >= width * height) {
                entities.erase(entities.begin() + it);
                it--;
                continue;
            }

            // Don't check neighbouring tiles because radius/width/height are 0
            if (!entities[it]->hitbox.active) {
                continue;
            }

            // Check more complicated collisions
            Pos2D tPos = entities[it]->cPos % cellSize;

            int ileft  = entities[it]->gPos.index(width, LEFT , 1);
            int iup    = entities[it]->gPos.index(width, UP   , 1);
            int iright = entities[it]->gPos.index(width, RIGHT, 1);
            int idown  = entities[it]->gPos.index(width, DOWN , 1);

            bool wleft  = (tiles[ileft ].type == WALL && tiles[ileft ].active);
            bool wup    = (tiles[iup   ].type == WALL && tiles[iup   ].active);
            bool wright = (tiles[iright].type == WALL && tiles[iright].active);
            bool wdown  = (tiles[idown ].type == WALL && tiles[idown ].active);

            int w, h;
            if (entities[it]->hitbox.type == HITBOX_CIRCLE) {
                w = entities[it]->hitbox.radius;
                h = entities[it]->hitbox.radius;
            }
            else {
                w = entities[it]->hitbox.width / 2;
                h = entities[it]->hitbox.height / 2;
            }

            // Left
            if (wleft && tPos.x < w) {
                entities.erase(entities.begin() + it);
                it--;
                continue;
            }

            // Up
            if (wup && tPos.y < h) {
                entities.erase(entities.begin() + it);
                it--;
                continue;
            }

            // Right
            if (wright && tPos.x > (cellSize - w)) {
                entities.erase(entities.begin() + it);
                it--;
                continue;
            }

            // Down
            if (wdown && tPos.y > (cellSize - h)) {
                entities.erase(entities.begin() + it);
                it--;
                continue;
            }
        }
    }

    { // Collide players
        std::vector<Hitbox> hitboxes;

        for (int i = 0; i < players.size(); i++) {
            Hitbox hb;
            hb.cPos = players[i].cPos;
            hb.radius = players[i].hitboxRadius;

            hitboxes.push_back(hb);
        }

        for (int it = 0; it < entities.size(); it++) {
            if (!entities[it]->collidesP) continue;

            for (int i = 0; i < hitboxes.size(); i++) {
                bool collided;

                // rectangle-rectangle collision impossible

                // circle-circle collision
                if (entities[it]->hitbox.type == HITBOX_CIRCLE) {
                    Pos2D dPos = hitboxes[i].cPos - entities[it]->hitbox.cPos;

                    int rsum = entities[it]->hitbox.radius + hitboxes[i].radius;

                    collided = ((dPos.x * dPos.x) + (dPos.y * dPos.y)) < (rsum * rsum);
                }

                // rectangle-circle collision
                else {
                    Pos2D dPos = hitboxes[i].cPos - entities[it]->hitbox.cPos;

                    int w = entities[it]->hitbox.width;
                    int h = entities[it]->hitbox.height;
                    int r = hitboxes[i].radius;

                    // Edge (horizontal)
                    if (absv(dPos.x) * 2 > w || absv(dPos.y) * 2 < h) {
                        collided = dPos.x < w / 2 + r;
                    }

                    // Edge (vertical)
                    else if (absv(dPos.x) * 2 < w || absv(dPos.y) * 2 > h) {
                        collided = dPos.y < h / 2 + r;
                    }

                    // Corners
                    else {
                        collided = (dPos.x - w / 2) * (dPos.x - w / 2) + (dPos.y - h / 2) * (dPos.y - h / 2) < r * r;
                    }
                }

                if (collided) {
                    Pos2D colgPos = hitboxes[i].cPos / cellSize;

                    if (entities[it]->get_type() == ENTITY_DAMAGE) {
                        healthMap[colgPos.index(width)].dInfo.push_back(entities[it]->damage);
                    }
                    else if (entities[it]->get_type() == ENTITY_HEAL) {
                        healthMap[colgPos.index(width)].hInfo.push_back(entities[it]->heal);
                    }

                    entities.erase(entities.begin() + it);
                    it--;
                    break;
                }
            }
        }
    }

    { // Collide objects
        std::vector<Hitbox> hitboxes;

        for (int i = 0; i < levelObjects.size(); i++) {
            if (levelObjects[i]->get_type() == OBJECT_DYNAMIC) {
                if (levelObjects[i]->hitbox.active) {
                    hitboxes.push_back(levelObjects[i]->hitbox);
                }
            }
        }

        for (int it = 0; it < entities.size(); it++) {
            if (!entities[it]->collidesE) continue;

            for (int i = 0; i < hitboxes.size(); i++) {
                bool collided;

                // rectangle-rectangle collision
                if (entities[it]->hitbox.type == HITBOX_RECTANGLE && hitboxes[i].type == HITBOX_RECTANGLE) {
                    Pos2D dPos = hitboxes[i].cPos - entities[it]->hitbox.cPos;

                    int w1 = entities[it]->hitbox.width;
                    int h1 = entities[it]->hitbox.height;
                    int w2 = hitboxes[i].width;
                    int h2 = hitboxes[i].height;

                    collided = (absv(dPos.x) < (w1 + w2) / 2) && (absv(dPos.y) < (h1 + h2) / 2);
                }

                // circle-circle collision
                else if (entities[it]->hitbox.type == HITBOX_CIRCLE && hitboxes[i].type == HITBOX_CIRCLE) {
                    Pos2D dPos = hitboxes[i].cPos - entities[it]->hitbox.cPos;

                    int rsum = entities[it]->hitbox.radius + hitboxes[i].radius;

                    collided = ((dPos.x * dPos.x) + (dPos.y * dPos.y)) < (rsum * rsum);
                }

                // rectangle-circle collision
                else {
                    Pos2D dPos = hitboxes[i].cPos - entities[it]->hitbox.cPos;

                    int w, h, r;
                    if (entities[it]->hitbox.type == HITBOX_CIRCLE) {
                        w = hitboxes[i].width;
                        h = hitboxes[i].height;
                        r = entities[it]->hitbox.radius;
                    }
                    else {
                        w = entities[it]->hitbox.width;
                        h = entities[it]->hitbox.height;
                        r = hitboxes[i].radius;
                    }

                    // Edge (horizontal)
                    if (absv(dPos.x) * 2 > w || absv(dPos.y) * 2 < h) {
                        collided = dPos.x < w/2 + r;
                    }

                    // Edge (vertical)
                    else if (absv(dPos.x) * 2 < w || absv(dPos.y) * 2 > h) {
                        collided = dPos.y < h/2 + r;
                    }

                    // Corners
                    else {
                        collided = (dPos.x - w/2) * (dPos.x - w/2) + (dPos.y - h/2) * (dPos.y - h/2) < r * r;
                    }
                }

                if (collided) {
                    Pos2D colgPos = hitboxes[i].cPos / cellSize;

                    if (entities[it]->get_type() == ENTITY_DAMAGE) {
                        healthMap[colgPos.index(width)].dInfo.push_back(entities[it]->damage);
                    }
                    else if (entities[it]->get_type() == ENTITY_HEAL) {
                        healthMap[colgPos.index(width)].hInfo.push_back(entities[it]->heal);
                    }

                    entities.erase(entities.begin() + it);
                    it--;
                    break;
                }
            }
        }
    }

    /*
    for (int i = 0; i < entities.size(); i++) {
        if (entities[i].collidesWithPlayer) {
            if (!entities[i].gold && !entities[i].item) {
                if (distance(player[0].cX, player[0].cY, entities[i].cX, entities[i].cY) <= entities[i].hitboxRadius + player[0].hitboxRadius) {
                    player[0].damage(entities[i].damage, "physical", "", commands);
                    player[0].heal(entities[i].heal);

                    entities.erase(entities.begin() + i);
                    i--;
                }
            }
            else if (entities[i].gold) {
                if (distance(player[0].cX, player[0].cY, entities[i].cX, entities[i].cY) <= entities[i].hitboxRadius + player[0].hitboxRadius) {
                    player[0].gold += entities[i].goldAmount;

                    entities.erase(entities.begin() + i);
                    i--;
                }
            }
            else if (entities[i].item) {
                if (player[0].items.size() < 5) {
                    if (distance(player[0].cX, player[0].cY, entities[i].cX, entities[i].cY) <= entities[i].hitboxRadius + player[0].hitboxRadius) {
                        player[0].items.push_back(Item(entities[i].itemName));

                        entities.erase(entities.begin() + i);
                        i--;
                    }
                }
            }
        }
    }

    for (int i = entities.size() - 1; i >= 0; i--) {
        int tileUp      = (entities[i].gY - 1) * width + entities[i].gX;
        int tileRight =  entities[i].gY         * width + entities[i].gX + 1;
        int tileDown  = (entities[i].gY + 1) * width + entities[i].gX;
        int tileLeft  =  entities[i].gY         * width + entities[i].gX - 1;

        if (tileUp > tiles.size() || tileDown > tiles.size() || tileLeft > tiles.size() || tileRight > tiles.size()) {
            entities.erase(entities.begin() + i);
            continue;
        }

        int tileX = entities[i].cX - entities[i].gX * cellWidth;
        int tileY = entities[i].cY - entities[i].gY * cellHeight;

        int index = entities[i].gY * width + entities[i].gX;

        if (tiles[index].type == "wall" && tiles[index].active) {
            entities.erase(entities.begin() + i);
            continue;
        }

        if        (tileX < entities[i].hitboxRadius && tiles[tileLeft].type == "wall" && tiles[tileLeft].active) {
            entities.erase(entities.begin() + i);
            continue;
        }
        else if (tileX > cellWidth - entities[i].hitboxRadius && tiles[tileRight].type == "wall" && tiles[tileRight].active) {
            entities.erase(entities.begin() + i);
            continue;
        }
        else if (tileY < entities[i].hitboxRadius && tiles[tileUp].type == "wall" && tiles[tileUp].active) {
            entities.erase(entities.begin() + i);
            continue;
        }
        else if (tileY > cellHeight - entities[i].hitboxRadius && tiles[tileDown].type == "wall" && tiles[tileDown].active) {
            entities.erase(entities.begin() + i);
            continue;
        }

        // OwO
        int tileUpLeft = (entities[i].gY - 1) * width + entities[i].gX - 1; /// ////
        int tileUpRight = (entities[i].gY - 1) * width + entities[i].gX + 1; /// ///
        int tileDownLeft = (entities[i].gY + 1) * width + entities[i].gX - 1; /// //
        int tileDownRight = (entities[i].gY + 1) * width + entities[i].gX + 1; /// /

        if        (distance(tileX, tileY, -1, -1) < entities[i].hitboxRadius && tiles[tileUpLeft].type == "wall") {
            entities.erase(entities.begin() + i);
            continue;
        }
        else if (distance(tileX, tileY, cellWidth, -1) < entities[i].hitboxRadius && tiles[tileUpRight].type == "wall") {
            entities.erase(entities.begin() + i);
            continue;
        }
        else if (distance(tileX, tileY, -1, cellHeight) < entities[i].hitboxRadius && tiles[tileDownLeft].type == "wall") {
            entities.erase(entities.begin() + i);
            continue;
        }
        else if (distance(tileX, tileY, cellWidth, cellHeight) < entities[i].hitboxRadius && tiles[tileDownRight].type == "wall") {
            entities.erase(entities.begin() + i);
            continue;
        }

        if (entities[i].gX > width - 1 || entities[i].gX < 0 || entities[i].gY > height - 1 || entities[i].gY < 0) {
            entities.erase(entities.begin() + i);
            continue;
        }
    }
    */
}

void Level::deal_damage(ScreenAnimations &screenAnimations)
{
    // Player damage


    // Level objects
    for (int it = 0; it < levelObjects.size(); it++) {
        int index = levelObjects[it]->gPos.index(width);
        if (!healthMap[index].active) continue;

        for (unsigned i = 0; i < healthMap[index].dInfo.size(); i++) {
            levelObjects[it]->damage(healthMap[index].dInfo[i]);
        }
    }

    // Clear damage map
    for (int i = 0; i < width * height; i++) {
        healthMap[i].reset();
    }
    /*
    int index;
    // Player
    index = player[0].gY * width + player[0].gX;
    if (damageMap[index].active) player[0].damage(damageMap[index], commands);

    // Crates
    for (int i = 0; i < crates.size(); i++) {
        index = crates[i].gY * width + crates[i].gX;
        if (damageMap[index].active) {
            crates[i].damage(tiles, damageMap[index].direction);
            if (damageMap[index].startShake) screenAnimations.start_shake(0);
        }
    }

    // Doors
    for (int i = doors.size() - 1; i >= 0; i--) {
        index = doors[i].gY * width + doors[i].gX;
        if (damageMap[index].active) {
            doors[i].damage(damageMap[index], commands);
            tiles[doors[i].gY * width + doors[i].gX].hasDoor = false;
            doors.erase(doors.begin() + i);
            doorCount--;
            if (damageMap[index].startShake) screenAnimations.start_shake(0);
        }
    }

    // Shooters
    for (int i = 0; i < shooters.size(); i++) {
        index = shooters[i].gY * width + shooters[i].gX;
        if (damageMap[index].active) {
            //shooters[i].damage(damageMap[index].enemyDamage);
            if (damageMap[index].startShake) screenAnimations.start_shake(0);
        }
    }

    // Barbarians
    for (int i = 0; i < barbarians.size(); i++) {
        index = barbarians[i].gY * width + barbarians[i].gX;
        if (damageMap[index].active && barbarians[i].targetable) {
            barbarians[i].damage(damageMap[index], commands);
            if (damageMap[index].startShake) screenAnimations.start_shake(0);
        }
    }

    // Wizards
    for (int i = 0; i < wizards.size(); i++) {
        index = wizards[i].gY * width + wizards[i].gX;
        if (damageMap[index].active && wizards[i].targetable) {
            wizards[i].damage(damageMap[index], commands);
            if (damageMap[index].startShake) screenAnimations.start_shake(0);
        }
    }

    // Paladins
    for (int i = 0; i < paladins.size(); i++) {
        index = paladins[i].gY * width + paladins[i].gX;
        if (damageMap[index].active && paladins[i].targetable) {
            paladins[i].damage(damageMap[index], commands);
            if (damageMap[index].startShake) screenAnimations.start_shake(0);
        }
    }

    // Zombies
    for (int i = 0; i < zombies.size(); i++) {
        index = zombies[i].gY * width + zombies[i].gX;
        if (damageMap[index].active && zombies[i].targetable) {
            zombies[i].damage(damageMap[index], commands);
            if (damageMap[index].startShake) screenAnimations.start_shake(0);
        }
    }

    // Liches
    for (int i = 0; i < liches.size(); i++) {
        index = liches[i].gY * width + liches[i].gX;
        if (damageMap[index].active && liches[i].targetable) {
            liches[i].damage(damageMap[index], commands);
            if (damageMap[index].startShake) screenAnimations.start_shake(0);
        }
    }

    // Wraiths
    for (int i = 0; i < wraiths.size(); i++) {
        index = wraiths[i].gY * width + wraiths[i].gX;
        if (damageMap[index].active && wraiths[i].targetable) {
            wraiths[i].damage(damageMap[index], commands);
            if (damageMap[index].startShake) screenAnimations.start_shake(0);
        }
    }

    // Start shake if player took damage
    if (player[0].tookDamage) screenAnimations.start_shake(player[0].damageAmount / 5);
    */
}

void Level::remove_destroyed_objects()
{
    for (unsigned i = 0; i < levelObjects.size(); i++) {
        if (levelObjects[i]->destroyed) {
            levelObjects.erase(levelObjects.begin() + i);
            i--;
        }
    }

    return;

    // Barbarians
    for (unsigned i = 0; i < barbarians.size(); i++) {
        if (barbarians[i].dead) {
            barbarians.erase(barbarians.begin() + i);
            i--;
        }
    }

    // Wizards
    for (unsigned i = 0; i < wizards.size(); i++) {
        if (wizards[i].dead) {
            wizards.erase(wizards.begin() + i);
            i--;
        }
    }
    
    // Paladins
    for (unsigned i = 0; i < paladins.size(); i++) {
        if (paladins[i].dead) {
            paladins.erase(paladins.begin() + i);
            i--;
        }
    }

    // Zombies
    for (unsigned i = 0; i < zombies.size(); i++) {
        if (zombies[i].dead) {
            zombies.erase(zombies.begin() + i);
            i--;
        }
    }

    // Liches
    for (unsigned i = 0; i < liches.size(); i++) {
        if (liches[i].dead) {
            liches.erase(liches.begin() + i);
            i--;
        }
    }

    // Wraiths
    for (unsigned i = 0; i < wraiths.size(); i++) {
        if (wraiths[i].dead) {
            wraiths.erase(wraiths.begin() + i);
            i--;
        }
    }
}

void Level::remove_finished_animations()
{
    for (unsigned i = 0; i < animations.size(); i++) {
        if (animations[i].finished) {
            animations.erase(animations.begin() + i);
            i--;
        }
    }
}

void Level::remove_finished_particles()
{
    for (unsigned i = 0; i < particles.size(); i++) {
        if (particles[i].finished) {
            particles.erase(particles.begin() + i);
            i--;
        }
    }
}

void Level::merge_gold_on_ground()
{
    /*
    int tCount = width * height;
    std::vector<int>  tileGold;
    std::vector<bool> goldRevealed;

    for (int i = 0; i < tCount; i++) {
        tileGold.push_back(0);
        goldRevealed.push_back(false);
    }

    for (int i = 0; i < entities.size(); i++) {
        if (entities[i].gold) {
            int index = entities[i].gY * width + entities[i].gX;

            tileGold[index] += entities[i].goldAmount;
            if (entities[i].revealed) goldRevealed[index] = true;

            entities.erase(entities.begin() + i);
            i--;
        }
    }

    for (int i = 0; i < tCount; i++) {
        if (tileGold[i] > 0) {
            entities.push_back(Entity(i % width, i / width, tileGold[i]));

            if (goldRevealed[i]) entities[entities.size() - 1].revealed = true;
        }
    }

    for (int i = 0; i < entities.size(); i++) {
        entities[i].visible = true;
    }
    */
}

void Level::update_minimap(Key &k, UserData &userData)
{
    for (unsigned i = 0; i < minimap.grid.size(); i++) {
        minimap.grid[i] = 0;
    }

    if (k == userData.keyBindings.CHANGE_MAP_SIZE) minimap.change_size();

    // Tiles
    for (int i = 0; i < _LEVEL_WIDTH * _LEVEL_HEIGHT; i++) {
        if      (tiles[i].type == WALL       && tiles[i].revealed) minimap.grid[i] = 1;
        else if (tiles[i].type == FLOOR      && tiles[i].revealed) minimap.grid[i] = 2;
        else if (tiles[i].type == BASE_FLOOR && tiles[i].revealed) minimap.grid[i] = 2;
        else if (tiles[i].type == WATER      && tiles[i].revealed) minimap.grid[i] = 3;
        else if (tiles[i].type == LAVA       && tiles[i].revealed) minimap.grid[i] = 4;
    }

    // Pressure plates
    for (unsigned i = 0; i < pressurePlates.size(); i++) {
        int index = pressurePlates[i].gY * width + pressurePlates[i].gX;
        if (pressurePlates[i].revealed) minimap.grid[index] = 5;
    }

    // Lamps
    for (unsigned i = 0; i < lamps.size(); i++) {
        int index = lamps[i].gY * width + lamps[i].gX;
        if (lamps[i].revealed) minimap.grid[index] = 5;
    }

    // Crates
    for (unsigned i = 0; i < crates.size(); i++) {
        int index = crates[i].gY * width + crates[i].gX;
        if (crates[i].revealed) minimap.grid[index] = 6;
    }

    // Traps

    // Doors
    for (unsigned i = 0; i < doors.size(); i++) {
        int index = doors[i].gY * width + doors[i].gX;
        if (doors[i].revealed) minimap.grid[index] = 8;
    }

    // Signs
    for (unsigned i = 0; i < signs.size(); i++) {
        int index = signs[i].gY * width + signs[i].gX;
        if (signs[i].revealed) minimap.grid[index] = 9;
    }

    // Gold and items
    for (unsigned i = 0; i < entities.size(); i++) {
        if (entities[i]->get_type() == ENTITY_GOLD) {
            //int index = entities[i].gY * width + entities[i].gX;
            //if (entities[i].revealed) minimap.grid[index] = 10;
        }
        if (entities[i]->get_type() == ENTITY_ITEM) {
            //int index = entities[i].gY * width + entities[i].gX;
            //if (entities[i].revealed) minimap.grid[index] = 11;
        }
    }

    // Barbarians
    for (unsigned i = 0; i < barbarians.size(); i++) {
        int index = barbarians[i].gY * width + barbarians[i].gX;
        if (barbarians[i].visible) minimap.grid[index] = 12;
    }

    // Wizards
    for (unsigned i = 0; i < wizards.size(); i++) {
        int index = wizards[i].gY * width + wizards[i].gX;
        if (wizards[i].visible) minimap.grid[index] = 12;
    }
    
    // Paladins
    for (unsigned i = 0; i < paladins.size(); i++) {
        int index = paladins[i].gY * width + paladins[i].gX;
        if (paladins[i].visible) minimap.grid[index] = 12;
    }

    // Zombies
    for (unsigned i = 0; i < zombies.size(); i++) {
        int index = zombies[i].gY * width + zombies[i].gX;
        if (zombies[i].visible) minimap.grid[index] = 12;
    }

    // Liches
    for (unsigned i = 0; i < liches.size(); i++) {
        int index = liches[i].gY * width + liches[i].gX;
        if (liches[i].visible) minimap.grid[index] = 12;
    }

    // Wraiths
    for (unsigned i = 0; i < wraiths.size(); i++) {
        int index = wraiths[i].gY * width + wraiths[i].gX;
        if (wraiths[i].visible) minimap.grid[index] = 12;
    }
    
    // Portals
    for (unsigned i = 0; i < portals.size(); i++) {
        int index = portals[i].entryPos.index(_LEVEL_WIDTH);
        if (portals[i].revealed) minimap.grid[index] = 13;
    }

    // Players
    for (unsigned i = 0; i < players.size(); i++) {
        int index = players[i].gPos.index(_LEVEL_WIDTH);
        minimap.grid[index] = 14;
    }
}

void Level::update_tile_info()
{
    // Clear info
    for (int i = 0; i < _LEVEL_WIDTH * _LEVEL_HEIGHT; i++) {
        tiles[i].occupied    = false;
        tiles[i].pressured    = false;
        tiles[i].pressuredW = false;
        tiles[i].pressuredS = false;
        tiles[i].pressuredG = false;
        tiles[i].powered    = false;
    }

// Occupation
    // Walls
    for (int i = 0; i < _LEVEL_WIDTH * _LEVEL_HEIGHT; i++) {
        if (tiles[i].type == WALL && tiles[i].active) tiles[i].occupied = true;
    }

    // Player
    tiles[players[0].gPos.index(_LEVEL_WIDTH)].occupied = true;
    if (players[0].inMA) 
        tiles[players[0].sgPos.index(_LEVEL_WIDTH, players[0].MAdirection, 1)].occupied = true;

    // Objects
    for (auto it : levelObjects) {
        tiles[it->gPos.index(width)].occupied = it->obstructive;
    }

    // Crates
    for (unsigned i = 0; i < crates.size(); i++) {
        tiles[crates[i].gY * width + crates[i].gX].occupied = true;

        if (crates[i].inMA) {
            if        (crates[i].MAdirection == "up")       tiles[(crates[i].sgY - 1) * width + crates[i].gX].occupied = true;
            else if (crates[i].MAdirection == "down")  tiles[(crates[i].sgY + 1) * width + crates[i].gX].occupied = true;
            else if (crates[i].MAdirection == "left")  tiles[crates[i].gY * width + crates[i].sgX - 1].occupied = true;
            else if (crates[i].MAdirection == "right") tiles[crates[i].gY * width + crates[i].sgX + 1].occupied = true;
        }
    }

    // Doors
    for (unsigned i = 0; i < doors.size(); i++) {
        tiles[doors[i].gY * width + doors[i].gX].occupied = true;
    }

    // Signs
    for (unsigned i = 0; i < signs.size(); i++) {
        tiles[signs[i].gY * width + signs[i].gX].occupied = true;
    }

    // Barbarians
    for (unsigned i = 0; i < barbarians.size(); i++) {
        if (!barbarians[i].hitboxActive) break;

        tiles[barbarians[i].gY * width + barbarians[i].gX].occupied = true;

        if (barbarians[i].inMA) {
            if        (barbarians[i].MAdirection == "up")       tiles[(barbarians[i].sgY - 1) * width + barbarians[i].gX].occupied = true;
            else if (barbarians[i].MAdirection == "down")  tiles[(barbarians[i].sgY + 1) * width + barbarians[i].gX].occupied = true;
            else if (barbarians[i].MAdirection == "left")  tiles[barbarians[i].gY * width + barbarians[i].sgX - 1].occupied = true;
            else if (barbarians[i].MAdirection == "right") tiles[barbarians[i].gY * width + barbarians[i].sgX + 1].occupied = true;
        }
    }

    // Wizards
    for (unsigned i = 0; i < wizards.size(); i++) {
        if (!wizards[i].hitboxActive) break;

        tiles[wizards[i].gY * width + wizards[i].gX].occupied = true;

        if (wizards[i].inMA) {
            if        (wizards[i].MAdirection == "up")    tiles[(wizards[i].sgY - 1) * width + wizards[i].gX].occupied = true;
            else if (wizards[i].MAdirection == "down")  tiles[(wizards[i].sgY + 1) * width + wizards[i].gX].occupied = true;
            else if (wizards[i].MAdirection == "left")  tiles[wizards[i].gY * width + wizards[i].sgX - 1].occupied = true;
            else if (wizards[i].MAdirection == "right") tiles[wizards[i].gY * width + wizards[i].sgX + 1].occupied = true;
        }
    }

    // Paladins
    for (unsigned i = 0; i < paladins.size(); i++) {
        if (!paladins[i].hitboxActive) break;

        tiles[paladins[i].gY * width + paladins[i].gX].occupied = true;

        if (paladins[i].inMA) {
            if        (paladins[i].MAdirection == "up")     tiles[(paladins[i].sgY - 1) * width + paladins[i].gX].occupied = true;
            else if (paladins[i].MAdirection == "down")  tiles[(paladins[i].sgY + 1) * width + paladins[i].gX].occupied = true;
            else if (paladins[i].MAdirection == "left")  tiles[paladins[i].gY * width + paladins[i].sgX - 1].occupied = true;
            else if (paladins[i].MAdirection == "right") tiles[paladins[i].gY * width + paladins[i].sgX + 1].occupied = true;
        }
    }

    // Zombies
    for (unsigned i = 0; i < zombies.size(); i++) {
        if (!zombies[i].hitboxActive) break;

        tiles[zombies[i].gY * width + zombies[i].gX].occupied = true;

        if (zombies[i].inMA) {
            if        (zombies[i].MAdirection == "up")    tiles[(zombies[i].sgY - 1) * width + zombies[i].gX].occupied = true;
            else if (zombies[i].MAdirection == "down")  tiles[(zombies[i].sgY + 1) * width + zombies[i].gX].occupied = true;
            else if (zombies[i].MAdirection == "left")  tiles[zombies[i].gY * width + zombies[i].sgX - 1].occupied = true;
            else if (zombies[i].MAdirection == "right") tiles[zombies[i].gY * width + zombies[i].sgX + 1].occupied = true;
        }
    }

    // Liches
    for (unsigned i = 0; i < liches.size(); i++) {
        if (!liches[i].hitboxActive) break;

        tiles[liches[i].gY * width + liches[i].gX].occupied = true;

        if (liches[i].inMA) {
            if        (liches[i].MAdirection == "up")       tiles[(liches[i].sgY - 1) * width + liches[i].gX].occupied = true;
            else if (liches[i].MAdirection == "down")  tiles[(liches[i].sgY + 1) * width + liches[i].gX].occupied = true;
            else if (liches[i].MAdirection == "left")  tiles[liches[i].gY * width + liches[i].sgX - 1].occupied = true;
            else if (liches[i].MAdirection == "right") tiles[liches[i].gY * width + liches[i].sgX + 1].occupied = true;
        }
    }

    // Wraiths
    for (unsigned i = 0; i < wraiths.size(); i++) {
        if (!wraiths[i].hitboxActive) break;

        tiles[wraiths[i].gY * width + wraiths[i].gX].occupied = true;

        if (wraiths[i].inMA) {
            if        (wraiths[i].MAdirection == "up")    tiles[(wraiths[i].sgY - 1) * width + wraiths[i].gX].occupied = true;
            else if (wraiths[i].MAdirection == "down")  tiles[(wraiths[i].sgY + 1) * width + wraiths[i].gX].occupied = true;
            else if (wraiths[i].MAdirection == "left")  tiles[wraiths[i].gY * width + wraiths[i].sgX - 1].occupied = true;
            else if (wraiths[i].MAdirection == "right") tiles[wraiths[i].gY * width + wraiths[i].sgX + 1].occupied = true;
        }
    }

    
// Pressure
    // Player
    if (!players[0].floating && players[0].onGround) {
        int index = players[0].gPos.index(_LEVEL_WIDTH);
        tiles[index].pressured  = true;
        tiles[index].pressuredW = true;
        tiles[index].pressuredS = true;

    }

    // Gold
    for (unsigned i = 0; i < entities.size(); i++) {
        if (entities[i]->get_type() == ENTITY_GOLD) {
            //int index = entities[i].gY * width + entities[i].gX;
            //tiles[index].pressured  = true;
            //tiles[index].pressuredW = true;
            //tiles[index].pressuredG = true;
        }
    }

    // Items
    for (unsigned i = 0; i < entities.size(); i++) {
        if (entities[i]->get_type() == ENTITY_ITEM) {
            //int index = entities[i].gY * width + entities[i].gX;
            //tiles[index].pressured  = true;
            //tiles[index].pressuredW = true;
        }
    }

    // Crates
    for (unsigned i = 0; i < crates.size(); i++) {
        if (crates[i].onGround) {
            int index = crates[i].gY * width + crates[i].gX;
            tiles[index].pressured  = true;
            tiles[index].pressuredW = true;
            tiles[index].pressuredS = true;
        }
    }

    // Barbarians
    for (unsigned i = 0; i < barbarians.size(); i++) {
        if (barbarians[i].onGround) {
            tiles[barbarians[i].gY * width + barbarians[i].gX].pressured  = true;
            tiles[barbarians[i].gY * width + barbarians[i].gX].pressuredW = true;
            tiles[barbarians[i].gY * width + barbarians[i].gX].pressuredS = true;
        }
    }

    // Wizards
    for (unsigned i = 0; i < wizards.size(); i++) {
        if (wizards[i].onGround) {
            tiles[wizards[i].gY * width + wizards[i].gX].pressured  = true;
            tiles[wizards[i].gY * width + wizards[i].gX].pressuredW = true;
            tiles[wizards[i].gY * width + wizards[i].gX].pressuredS = true;
        }
    }
    
    // Paladins
    for (unsigned i = 0; i < paladins.size(); i++) {
        if (paladins[i].onGround) {
            tiles[paladins[i].gY * width + paladins[i].gX].pressured  = true;
            tiles[paladins[i].gY * width + paladins[i].gX].pressuredW = true;
            tiles[paladins[i].gY * width + paladins[i].gX].pressuredS = true;
        }
    }

    // Zombies
    for (unsigned i = 0; i < zombies.size(); i++) {
        if (zombies[i].onGround) {
            tiles[zombies[i].gY * width + zombies[i].gX].pressured  = true;
            tiles[zombies[i].gY * width + zombies[i].gX].pressuredW = true;
            tiles[zombies[i].gY * width + zombies[i].gX].pressuredS = true;
        }
    }

    // Liches (never on ground)
    for (unsigned i = 0; i < liches.size(); i++) {
        /*
        if (liches[i].onGround) {
            tiles[liches[i].gY * width + liches[i].gX].pressured  = true;
            tiles[liches[i].gY * width + liches[i].gX].pressuredW = true;
            tiles[liches[i].gY * width + liches[i].gX].pressuredS = true;
        }
        */
    }

    // Wraiths (never on ground)
    for (unsigned i = 0; i < wraiths.size(); i++) {
        /*
        if (liches[i].onGround) {
        tiles[liches[i].gY * width + liches[i].gX].pressured  = true;
        tiles[liches[i].gY * width + liches[i].gX].pressuredW = true;
        tiles[liches[i].gY * width + liches[i].gX].pressuredS = true;
        }
        */
    }

// Power
    // Pressure plates
    for (unsigned i = 0; i < pressurePlates.size(); i++) {
        if (pressurePlates[i].pressed) {
            int index = pressurePlates[i].gY * width + pressurePlates[i].gX;
            tiles[index].powered = true;
        }
    }
}

void Level::update_wires()
{
    // Clear wire power
    for (int i = 0; i < _LEVEL_WIDTH * _LEVEL_HEIGHT; i++) {
        if (wires[i].leftPowered) wires[i].leftPowered = false;
        if (wires[i].rightPowered) wires[i].rightPowered = false;
        if (wires[i].redPowered) wires[i].redPowered = false;
        if (wires[i].bluePowered) wires[i].bluePowered = false;
        if (wires[i].greenPowered) wires[i].greenPowered = false;
        if (wires[i].yellowPowered) wires[i].yellowPowered = false;
    }

    // Power wires
    for (int i = 0; i < _LEVEL_WIDTH * _LEVEL_HEIGHT; i++) {
        if (tiles[i].powered) {
            if (!wires[i].gate) {
                if (wires[i].red && !wires[i].redPowered) {
                    //wires[i].power(wires, "", "red", width, height);
                }
                if (wires[i].blue && !wires[i].bluePowered) {
                    //wires[i].power(wires, "", "blue", width, height);
                }
                if (wires[i].green && !wires[i].greenPowered) {
                    //wires[i].power(wires, "", "green", width, height);
                }
                if (wires[i].yellow && !wires[i].yellowPowered) {
                    //wires[i].power(wires, "", "yellow", width, height);
                }
            }
        }
    }
}

void Level::update_lighting()
{
    // Create and fill a temporary light level array
    int *tileLights = new int[tiles.size()]{ 0 };

    // Clear tile lighting
    for (int i = 0; i < _LEVEL_WIDTH * _LEVEL_HEIGHT; i++) {
        tiles[i].directlyLit = false;
    }

    // Set limits for light which has no effect outside of player vision
    Pos2D limitTopLeft     = (visionCenterPos - Pos2D(cellSize * 25)) / cellSize;
    Pos2D limitBottomRight = (visionCenterPos + Pos2D(cellSize * 25)) / cellSize;

    // Add level size limits to limits
    RECT bounds = { 0, 0, _LEVEL_WIDTH, _LEVEL_HEIGHT };
    add_bounds(limitTopLeft    , bounds);
    add_bounds(limitBottomRight, bounds);

    // Structure for light emitting objects
    struct LightSource
    {
        Pos2D cPos;
        int lightLevel;
        int lightStrength;

        LightSource(Pos2D cPos, int lightLevel) : cPos(cPos), lightLevel(lightLevel), lightStrength(lightLevel * cellSize) {}
    };

    // Add light sources
    std::vector<LightSource> lightSources;

        // Players
    for (unsigned i = 0; i < players.size(); i++) {
        lightSources.push_back(LightSource(players[i].cPos              , players[i].lightLevel));
        lightSources.push_back(LightSource(players[i].cPos - Pos2D(1, 0), players[i].lightLevel));
        lightSources.push_back(LightSource(players[i].cPos - Pos2D(0, 1), players[i].lightLevel));
        lightSources.push_back(LightSource(players[i].cPos - Pos2D(1, 1), players[i].lightLevel));
    }

    // Light up all transparent tiles around light sources
    for (unsigned i = 0; i < lightSources.size(); i++) {
        // Each light source emits light rays around itself which light up tiles they touch
        const int lightRays = 90;

        // Variables for light ray calculations
        const float PI = 3.14159265f;
        int radius = lightSources[i].lightStrength;

        Pos2D rsPos = lightSources[i].cPos; // Ray start position
        Pos2D rePos;                        // Ray end position
        Pos2D pVec;                         // Path vector

        // Store light levels of tiles for calculation before adding them to the total
        int *lightLevels = new int[tiles.size()]{ 0 };

        // Calculate ray path
        for (int d = 0; d < lightRays; d++) {
            // Check if points on line between the corner and light source are on a non-transparent tile
            // Check every *pixelGap* pixels to save on computing power and because absolute precision isn't needed
            const int pixelGap = 5;

            // Calculate radians
            float curRadians = (360 / lightRays) * d * (PI / 180);

            // Calculate end coordinates
            rePos.x = static_cast<int>(round(radius * cos(curRadians))) + rsPos.x;
            rePos.y = static_cast<int>(round(radius * sin(curRadians))) + rsPos.y;

            // Calculate path vector
            pVec = rePos - rsPos;

            // Set distance
            const int dist = radius;
            int distTravelled = 0;

            // Variables for calculating tile light level
            int currentTile      = (rsPos / cellSize).index(_LEVEL_WIDTH); // Tile index on which the ray is currently on
            int tileRayHitPoints = 0;                                      // Number of times a tile has been hit by a ray;
            int tileLightLevel   = 0;                                      // Total light level from ray hits

            Pos2D pgPos = rsPos / cellSize; // Previous grid position

            while (distTravelled <= dist) {
                float progress = (float)distTravelled / dist;

                // Calculate current grid Position and index
                Pos2D cgPos = (rsPos + (pVec * progress)) / cellSize; // Current grid position
                int index = cgPos.index(_LEVEL_WIDTH);
                int pindex;

                // If the ray entered another tile, calculate the final previous tile's light level and reset variables
                if (index != currentTile) {
                    add_bounds(tileRayHitPoints, 1, 100);
                    int averageLightLevel = tileLightLevel / tileRayHitPoints;
                    
                    // Pick brightest ray
                    pindex = pgPos.index(_LEVEL_WIDTH);
                    if (averageLightLevel > lightLevels[pindex]) {
                        lightLevels[pindex] = averageLightLevel;
                    }

                    // Reset variables
                    tileRayHitPoints = 0;
                    tileLightLevel   = 0;
                    currentTile      = cgPos.index(_LEVEL_WIDTH);

                    pgPos = cgPos;
                }

                // If current tile isn't transparent, stop the light ray
                if (!tiles[cgPos.index(_LEVEL_WIDTH)].transparent) {
                    break;
                }

                // Add light and hit points
                tileLightLevel += dist - distTravelled;
                tileRayHitPoints++;

                // Advance position
                distTravelled += pixelGap;
            }
        }

        // Add light levels to tiles
        for (unsigned j = 0; j < tiles.size(); j++) {
            tileLights[j] += lightLevels[j];
        }

        // Release light level memory
        delete [] lightLevels;
        lightLevels = nullptr;
    }

    // Set directly lit tiles
    for (unsigned i = 0; i < tiles.size(); i++)
        if (tileLights[i] > 0)
            tiles[i].directlyLit = true;

    // Add smooth lighting
    for (int y = limitTopLeft.y; y < limitBottomRight.y; y++) {
        for (int x = limitTopLeft.x; x < limitBottomRight.x; x++) {
            // Set current tile index
            int index = y * _LEVEL_WIDTH + x;

            // Check if tile is transparent and not lit by light sources
            if (!tiles[index].transparent || tiles[index].directlyLit)
                continue;

            // Sum light from tiles around it
            for (int dir = 0; dir < 4; dir++) {

                // Create a temporary tile near the original one
                Pos2D tile = tiles[index].gridPos;
                tile.move(static_cast<Direction>(dir), 1);

                // Check if the tile is in map bounds, transparent, lit by light sources and bright enough
                if (add_bounds(tile, { 0, 0, _LEVEL_WIDTH - 1, _LEVEL_HEIGHT - 1 }))
                    if (tiles[tile.index(_LEVEL_WIDTH)].transparent && tiles[tile.index(_LEVEL_WIDTH)].directlyLit)
                        if (tileLights[tile.index(_LEVEL_WIDTH)] >= 384)
                            tileLights[index] += tileLights[tile.index(_LEVEL_WIDTH)];
            }

            // Get half average
            tileLights[index] /= 8;
            continue;
        }
    }
    
    // Light up non-transparent tiles
    for (int y = limitTopLeft.y; y < limitBottomRight.y; y++) {
        for (int x = limitTopLeft.x; x < limitBottomRight.x; x++) {
            // Set current tile index
            int index = y * _LEVEL_WIDTH + x;

            // Skip if tile is transparent
            if (tiles[index].transparent) continue;

            // Sum up tile light level around a tile
            for (int dir = 0; dir < 4; dir++) {

                Pos2D tile = tiles[index].gridPos;
                tile.move(static_cast<Direction>(dir), 1);

                // Check if the tile is in map bounds and transparent
                if (add_bounds(tile, { 0, 0, _LEVEL_WIDTH - 1, _LEVEL_HEIGHT - 1 }))
                    if (tiles[tile.index(_LEVEL_WIDTH)].transparent)
                        tileLights[index] += tileLights[tile.index(_LEVEL_WIDTH)];

                // Check corner tile
                Direction dir_ = static_cast<Direction>(dir);
                rotate_direction(dir_, 1);
                tile.move(dir_, 1);

                // Check if the tile is in map bounds, transparent and bright enough
                if (add_bounds(tile, { 0, 0, _LEVEL_WIDTH - 1, _LEVEL_HEIGHT - 1 }))
                    if (tiles[tile.index(_LEVEL_WIDTH)].transparent)
                        if (tileLights[tile.index(_LEVEL_WIDTH)] >= 768)
                            tileLights[index] += tileLights[tile.index(_LEVEL_WIDTH)];
            }

            tileLights[index] /= 4;
        }
    }

    // Set tile values
    for (unsigned i = 0; i < tiles.size(); i++) {
        // Set tile light level
        tiles[i].set_light_level(tileLights[i]);

        // Fill temporary values to calculate final brightness
        tileLights[i] = tiles[i].lightLevel;
    }

    // Final adjustments
    for (int y = limitTopLeft.y; y < limitBottomRight.y; y++) {
        for (int x = limitTopLeft.x; x < limitBottomRight.x; x++) {
            // Set current tile index
            int index = y * _LEVEL_WIDTH + x;

            // Cap light level to 768
            add_bounds(tileLights[index], 0, 768);

            // Convert light level to a value between 1 and 0
            tiles[index].brightness = tileLights[index] / 768.0f;

            // Set side brightness
            if (tiles[index].type == WALL) tiles[index].sideBrightness = tiles[index].brightness;

            // Create a checker pattern on floor
            if ((x + y) % 2 == 0 && (tiles[index].type == FLOOR || tiles[index].type == BASE_FLOOR)) {
                tiles[index].brightness *= 0.85f;
            }
        }
    }
    
    // Release tile light memory
    delete [] tileLights;
    tileLights = nullptr;


    update_visibility();
}

void Level::update_visibility()
{
    // Tiles
    for (int i = 0; i < _LEVEL_WIDTH * _LEVEL_HEIGHT; i++) {
        // Set visible tiles as visible
        if (tiles[i].brightness > 0) tiles[i].visible = true;
        else tiles[i].visible = false;

        if (tiles[i].sideBrightness > 0) tiles[i].sideVisible = true;
        else tiles[i].sideVisible = false;

        // Set visible tiles as revealed
        if (tiles[i].visible) tiles[i].revealed = true;
        if (i + width < _LEVEL_WIDTH * _LEVEL_HEIGHT && tiles[i + width].lightLevel > 0) tiles[i].sideRevealed = true;

        // Set the sides of walls, which have a wall below them, as not visible
        if (tiles[i].gridPos.y < height - 1 && tiles[i].gridPos.x > 0 && tiles[i].gridPos.x < width - 1) {
            if (tiles[i + width].type == WALL && tiles[i + width].active) {
                tiles[i].sideVisible = false;
            }
        }
        else {
            tiles[i].sideVisible = false;
        }
    }

    // Portals
    for (unsigned i = 0; i < portals.size(); i++)
    {
        int arrayPos = portals[i].entryPos.index(_LEVEL_WIDTH);

        if (tiles[arrayPos].visible) {
            portals[i].visible  = true;
            portals[i].revealed = true;
        }
        else
            portals[i].visible = false;
    }

    // Objects
    for (auto it : levelObjects) {
        int arrayPos = it->gPos.index(width);

        if (tiles[arrayPos].visible) {
            it->visible = true;
            it->revealed = true;
        }
        else {
            it->visible = false;
        }
    }

    // Entities
    for (unsigned i = 0; i < entities.size(); i++) {
        int arrayPos = entities[i]->gPos.index(width);

        if (tiles[arrayPos].visible) {
            entities[i]->visible  = true;
            entities[i]->revealed = true;
        }
        else
            entities[i]->visible = false;
    }
    /*
    // Crates
    for (unsigned i = 0; i < crates.size(); i++) {
        int arrayPos = crates[i].gY * width + crates[i].gX;

        if (tiles[arrayPos].visible) {
            crates[i].visible  = true;
            crates[i].revealed = true;
        }
        else {
            crates[i].visible = false;
        }
    }

    // Pressure plates
    for (unsigned i = 0; i < pressurePlates.size(); i++) {
        int arrayPos = pressurePlates[i].gY * width + pressurePlates[i].gX;

        if (tiles[arrayPos].visible) {
            pressurePlates[i].visible  = true;
            pressurePlates[i].revealed = true;
        }
        else
            pressurePlates[i].visible = false;
    }

    // Lamps
    for (unsigned i = 0; i < lamps.size(); i++) {
        int arrayPos = lamps[i].gY * width + lamps[i].gX;

        if (tiles[arrayPos].visible) {
            lamps[i].visible  = true;
            lamps[i].revealed = true;
        }
        else
            lamps[i].visible = false;
    }

    // Doors
    for (unsigned i = 0; i < doors.size(); i++) {
        int arrayPos = doors[i].gY * width + doors[i].gX;

        if (tiles[arrayPos].visible) {
            doors[i].visible  = true;
            doors[i].revealed = true;
        }
        else
            doors[i].visible = false;
    }

    // Signs
    for (unsigned i = 0; i < signs.size(); i++) {
        int arrayPos = signs[i].gY * width + signs[i].gX;

        if (tiles[arrayPos].visible) {
            signs[i].visible  = true;
            signs[i].revealed = true;
        }
        else {
            signs[i].visible = false;
        }
    }

    // Shooters
    for (unsigned i = 0; i < shooters.size(); i++) {
        if (shooters[i].spawning) {
            shooters[i].visible = false;
            continue;
        }

        int arrayPos = shooters[i].gY * width + shooters[i].gX;

        if (tiles[arrayPos].visible) {
            shooters[i].visible  = true;
            shooters[i].active   = true;
            shooters[i].revealed = true;
        }
        else {
            shooters[i].visible = false;
        }
    }

    // Barbarians
    for (unsigned i = 0; i < barbarians.size(); i++) {
        if (barbarians[i].spawning) {
            barbarians[i].visible = false;
            continue;
        }

        int arrayPos = barbarians[i].gY * width + barbarians[i].gX;

        if (tiles[arrayPos].visible) {
            barbarians[i].visible  = true;
            barbarians[i].active   = true;
            barbarians[i].revealed = true;
        }
        else {
            barbarians[i].visible = false;
        }
    }

    // Wizards
    for (unsigned i = 0; i < wizards.size(); i++) {
        if (wizards[i].spawning) {
            wizards[i].visible = false;
            continue;
        }

        int arrayPos = wizards[i].gY * width + wizards[i].gX;

        if (tiles[arrayPos].visible) {
            wizards[i].visible  = true;
            wizards[i].active   = true;
            wizards[i].revealed = true;
        }
        else {
            wizards[i].visible = false;
        }
    }
    
    // Paladins
    for (unsigned i = 0; i < paladins.size(); i++) {
        if (paladins[i].spawning) {
            paladins[i].visible = false;
            continue;
        }

        int arrayPos = paladins[i].gY * width + paladins[i].gX;

        if (tiles[arrayPos].visible) {
            paladins[i].visible  = true;
            paladins[i].active   = true;
            paladins[i].revealed = true;
        }
        else {
            paladins[i].visible = false;
        }
    }

    // Zombies
    for (unsigned i = 0; i < zombies.size(); i++) {
        if (zombies[i].spawning) {
            zombies[i].visible = false;
            continue;
        }
        int arrayPos = zombies[i].gY * width + zombies[i].gX;

        if (tiles[arrayPos].visible) {
            zombies[i].visible  = true;
            zombies[i].active   = true;
            zombies[i].revealed = true;
        }
        else {
            zombies[i].visible = false;
        }
    }

    // Liches
    for (unsigned i = 0; i < liches.size(); i++) {
        if (liches[i].spawning) {
            liches[i].visible = false;
            continue;
        }

        int arrayPos = liches[i].gY * width + liches[i].gX;

        if (tiles[arrayPos].visible) {
            liches[i].visible  = true;
            liches[i].active   = true;
            liches[i].revealed = true;
        }
        else {
            liches[i].visible = false;
        }
    }

    // Wraiths
    for (unsigned i = 0; i < wraiths.size(); i++) {
        if (wraiths[i].spawning) {
            wraiths[i].visible = false;
            continue;
        }

        int arrayPos = wraiths[i].gY * width + wraiths[i].gX;

        if (tiles[arrayPos].visible) {
            wraiths[i].visible  = true;
            wraiths[i].active   = true;
            wraiths[i].revealed = true;
        }
        else {
            wraiths[i].visible = false;
        }
    }
    */
}

void Level::finish_level()
{
    if (players[0].dead) {
        playerDied         = true;
        inDeathScreen      = true;
        startedDeathEffect = false;
        deathScreenStart   = maintime::now();

        newMode = "";

        levelEndTime = maintime::now();

        inEDFA = false;

        //duration<float> levelSeconds = levelEndTime - levelStartTime;
        int levelSecInt = static_cast<int>((levelEndTime - levelStartTime).get_duration(SECONDS));
        int levelS = levelSecInt % 60;
        int levelM = levelSecInt / 60;

        std::ostringstream ss;
        ss << levelM << ":" << levelS;
        levelTime = ss.str();
    }

    for (unsigned i = 0; i < portals.size(); i++) {
        for (unsigned j = 0; j < players.size(); j++) {
            if (players[j].gPos == portals[i].entryPos && players[j].inMA == false) {
                // Set new player locations
                players[0].gPos = portals[i].exitPos;
                if (players.size() > 1) players[1].gPos = portals[i].exitPos;

                // Set new level and stage
                saveData.currentLevel = portals[i].newLevel % levelsInStage;
                saveData.currentStage = portals[i].newLevel / levelsInStage;

                // Save and then reload current game state
                saveData.write_to_file(this);
                load_save_file(saveData.saveName + ".txt");

                return;
                /*
                reachedExit         = true;
                inEndScreen         = true;
                startedEndEffect = false;
                endScreenStart     = system_clock::now();

                newMode = "";

                levelEndTime = system_clock::now();

                inEDFA = false;

                duration<float> levelSeconds = levelEndTime - levelStartTime;
                int levelSecInt = round(levelSeconds.count());
                int levelS = levelSecInt % 60;
                int levelM = levelSecInt / 60;

                std::ostringstream ss;
                ss << levelM << ":" << levelS;
                levelTime = ss.str();
                */
            }
        }
    }
}

void Level::toggle_pause()
{
    if (!paused) {
        paused = true;
    }
    else {
        paused = false;
    }
}
