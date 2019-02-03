#include "Game.h"

#include "GlobalData.h"

#include "MainWindow.h"

#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>

void Game::ComposeGame()
{
    if (!gameStateData.levelLoaded) return;

    //float pauseDim = 1.0f;
    //if (level.paused) pauseDim = 0.5f;

    Pos2D centerPos(gfx.ScreenWidth / 2, gfx.ScreenHeight / 2);
    Pos2D startPos(centerPos - level.players[0].cPos);

    // Show death screen
    if (level.inDeathScreen && !level.startedDeathEffect) {
        screenAnimations.start_death_effect();
        level.startedDeathEffect = true;
        return;
    }
    else if (level.inDeathScreen) {
        /// draw_death_screen(centerX, centerY);
        return;
    }

    // Show end screen without rendering the game if fade is 100% to minimize lag
    if (level.inEndScreen && level.startedEndEffect) {
        duration<float> elapsed_seconds = system_clock::now() - screenAnimations.EEstart;

        if (elapsed_seconds.count() >= 1.0f) {
            /// draw_end_screen(centerX, centerY);
            return;
        }
    }

    /// apply_shake(startX, startY);

/// Draw floor
    for (int i = 0; i < _LEVEL_WIDTH * _LEVEL_HEIGHT; i++) {
        SpriteDrawData sdd;
        sdd.brightness = level.tiles[i].brightness;

        if (level.tiles[i].type != WALL) {
            if (level.tiles[i].brightness > 0.0f) {
                gfx.DrawSprite(startPos + level.tiles[i].gridPos * cellSize, sprites[DIRT], sdd);
            }
        }
        else if (level.tiles[i].type == WALL && !level.tiles[i].active) {
            if (level.tiles[i].brightness > 0.0f) {
                gfx.DrawSprite(startPos + level.tiles[i].gridPos * cellSize, sprites[DIRT], sdd);
            }
        }

        /* UNCOMMENT LATER
        // Draw floor edge shadows
        if (level.tiles[i].visible && (level.tiles[i].type != "wall" || (level.tiles[i].type == "wall" && !level.tiles[i].active))) {
            if (level.tiles[i - level.width].type == "wall" && level.tiles[i - level.width].active) {
                gfx.DrawFloorEdgeShadows(startX + level.tiles[i].x * cellWidth, startY + level.tiles[i].y * cellHeight, "up");
            }
            if (level.tiles[i - 1].type == "wall" && level.tiles[i - 1].active) {
                gfx.DrawFloorEdgeShadows(startX + level.tiles[i].x * cellWidth, startY + level.tiles[i].y * cellHeight, "left");
            }
            if (level.tiles[i + 1].type == "wall" && level.tiles[i + 1].active) {
                gfx.DrawFloorEdgeShadows(startX + level.tiles[i].x * cellWidth, startY + level.tiles[i].y * cellHeight, "right");
            }
        }
        */
    }

    // Draw ground animations
    for (int i = 0; i < level.animations.size(); i++) {
        if (level.animations[i].ground) {
            SpriteDrawData sdd;
            sdd.irect = level.animations[i].spriteRegion;
            sdd.ira = true;

            gfx.DrawSprite(startPos + level.animations[i].sPos, sprites[level.animations[i].spriteIndex], sdd);
        }
    }

/// Draw level elements top to botton
    for (int y = 0; y < level.height; y++) {
        SpriteDrawData sdd;

        // Walls
        for (int c = y * level.width; c < (y + 1) * level.width; c++) {
            if (level.tiles[c].type == WALL && level.tiles[c].brightness > 0.0f) {
                sdd.brightness = level.tiles[c].brightness;

                if (level.tiles[c].active) {
                    if (level.tiles[c].sideVisible && level.tiles[c].sideRevealed) {
                        gfx.DrawSprite(startPos + level.tiles[c].gridPos * cellSize + Pos2D(0, cellHeight / 2), sprites[STONE_SIDE], sdd);
                    }
                    if (level.tiles[c].visible) {
                        gfx.DrawSprite(startPos + level.tiles[c].gridPos * cellSize - Pos2D(0, cellHeight / 2), sprites[STONE], sdd);
                    }
                }
                else {
                    sdd.transparency = 0.75f;

                    if (level.tiles[c].sideVisible && level.tiles[c].sideRevealed) {
                        gfx.DrawSprite(startPos + level.tiles[c].gridPos * cellSize + Pos2D(0, cellHeight / 2), sprites[STONE_SIDE], sdd);
                    }
                    if (level.tiles[c].visible) {
                        gfx.DrawSprite(startPos + level.tiles[c].gridPos * cellSize - Pos2D(0, cellHeight / 2), sprites[STONE], sdd);
                    }
                }
            }
        }

        // Portals
        for (int i = 0; i < level.portals.size(); i++) {
            if (level.portals[i].entryPos.y == y && level.portals[i].visible) {
                SpriteDrawData sddp;
                sddp.irect = level.portals[i].spriteRegion;
                sddp.ira = true;

                gfx.DrawSprite(startPos + level.portals[i].entryPos * cellSize + Pos2D(0, 24), sprites[EXIT_GREEN], sddp);
            }
        }

        //sprites[EXIT_GREEN].GetRegion(level.portals[0].spriteRegion);

        /* UNCOMMENT LATER
        // Pressure plates
        for (int i = 0; i < level.pressurePlates.size(); i++) {
            if (level.pressurePlates[i].gY == y && level.pressurePlates[i].visible) {
                if (level.pressurePlates[i].pressed) {
                    std::string spriteInfo = level.pressurePlates[i].type + "_plate_pressed";
                    send_draw_info(startX + level.pressurePlates[i].sX, startY + level.pressurePlates[i].sY, spriteInfo, 1.0f * pauseDim, s_wiring);
                }
                else {
                    std::string spriteInfo = level.pressurePlates[i].type + "_plate";
                    send_draw_info(startX + level.pressurePlates[i].sX, startY + level.pressurePlates[i].sY, spriteInfo, 1.0f * pauseDim, s_wiring);
                }
            }
        }
        */

        /* UNCOMMENT LATER
        // Lamps
        for (int i = 0; i < level.lamps.size(); i++) {
            if (level.lamps[i].gY == y && level.lamps[i].visible) {
                if (level.lamps[i].powered) {
                    std::string spriteInfo = "lamp_" + level.lamps[i].type + "_on";
                    send_draw_info(startX + level.lamps[i].sX, startY + level.lamps[i].sY, spriteInfo, 1.0f * pauseDim, s_wiring);
                }
                else {
                    std::string spriteInfo = "lamp_" + level.lamps[i].type + "_off";
                    send_draw_info(startX + level.lamps[i].sX, startY + level.lamps[i].sY, spriteInfo, 1.0f * pauseDim, s_wiring);
                }
            }
        }
        */

        // Entities (upper half of a tile)
        for (int i = 0; i < level.entities.size(); i++) {
            if (level.entities[i]->gPos.y == y) {
                int cellY = level.entities[i]->cPos.y - (y * cellSize);

                if (cellY < cellHeight / 2 && level.entities[i]->visible) {
                    //send_draw_info_IRECT(startPos + level.entities[i]->sPos, 1.0f, sprites[level.entities[i]->spriteIndex], level.entities[i]->spriteRegion);
                    gfx.DrawSprite(startPos + level.entities[i]->sPos, sprites[level.entities[i]->spriteIndex]);
                }
            }
        }

        // Players
        for (int i = 0; i < level.players.size(); i++) {
            if (level.players[0].gPos.y == y) {
                gfx.DrawSprite(startPos + level.players[0].sPos, sprites[level.players[0].finalSprite]);
            }
        }

        // Objects
        for (unsigned i = 0; i < level.staticObjects.size(); i++) {
            if (level.staticObjects[i].gPos.y == y && level.staticObjects[i].visible) {
                gfx.DrawSprite(startPos + level.staticObjects[i].sPos, sprites[level.staticObjects[i].currentSprite]);
            }
        }
        for (unsigned i = 0; i < level.dynamicObjects.size(); i++) {
            if (level.dynamicObjects[i].gPos.y == y && level.dynamicObjects[i].visible) {
                gfx.DrawSprite(startPos + level.dynamicObjects[i].sPos, sprites[level.dynamicObjects[i].currentSprite]);
            }
        }

        /* UNCOMMENT LATER
        // Crates
        for (int i = 0; i < level.crates.size(); i++) {
            if (level.crates[i].gY == y && level.crates[i].visible) {
                send_draw_info(startX + level.crates[i].sX, startY + level.crates[i].sY, "crate", 1.0f * pauseDim, s_wiring);
            }
        }

        // Doors
        for (int i = 0; i < level.doors.size(); i++) {
            if (level.doors[i].gY == y && level.doors[i].visible) {
                std::string spriteInfo = "door_" + level.doors[i].facing;
                send_draw_info(startX + level.doors[i].sX, startY + level.doors[i].sY, spriteInfo, 1.0f * pauseDim, s_doors);
            }
        }

        // Signs
        for (int i = 0; i < level.signs.size(); i++) {
            if (level.signs[i].gY == y && level.signs[i].visible) {
                std::string spriteInfo = "sign";
                send_draw_info(startX + level.signs[i].sX, startY + level.signs[i].sY, spriteInfo, 1.0f * pauseDim, s_other);
            }
        }

        // Shooters
        for (int i = 0; i < level.shooters.size(); i++) {
            if (level.shooters[i].gY == y && level.shooters[i].visible) {
                std::string spriteInfo = "shooter_" + level.shooters[i].type + "_" + level.shooters[i].facing + "_1";
                send_draw_info(startX + level.shooters[i].sX, startY + level.shooters[i].sY, spriteInfo, 1.0f * pauseDim, s_shooter);
            }
        }

        // Barbarians
        for (int i = 0; i < level.barbarians.size(); i++) {
            if (level.barbarians[i].gY == y && level.barbarians[i].visible) {
                std::string spriteInfo = "barbarian_" + level.barbarians[i].type + "_" + level.barbarians[i].currentAction + "_" + level.barbarians[i].spriteFacing + "_" + level.barbarians[i].spriteNumber;
                send_draw_info(startX + level.barbarians[i].sX, startY + level.barbarians[i].sY, spriteInfo, 1.0f * pauseDim, s_barbarian);
            }
        }

        // Wizards
        for (int i = 0; i < level.wizards.size(); i++) {
            if (level.wizards[i].gY == y && level.wizards[i].visible) {
                std::string spriteInfo = "wizard_" + level.wizards[i].type + "_" + level.wizards[i].currentAction + "_" + level.wizards[i].spriteFacing + "_" + level.wizards[i].spriteNumber;
                send_draw_info(startX + level.wizards[i].sX, startY + level.wizards[i].sY, spriteInfo, 1.0f * pauseDim, s_wizard);
            }
        }

        // Paladins
        for (int i = 0; i < level.paladins.size(); i++) {
            if (level.paladins[i].gY == y && level.paladins[i].visible) {
                std::string spriteInfo = "paladin_" + level.paladins[i].type + "_" + level.paladins[i].currentAction + "_" + level.paladins[i].spriteFacing + "_" + level.paladins[i].spriteNumber;
                send_draw_info(startX + level.paladins[i].sX, startY + level.paladins[i].sY, spriteInfo, 1.0f * pauseDim, s_paladin);
            }
        }

        // Zombies
        for (int i = 0; i < level.zombies.size(); i++) {
            if (level.zombies[i].gY == y && level.zombies[i].visible) {
                std::string spriteInfo = "zombie_" + level.zombies[i].type + "_" + level.zombies[i].currentAction + "_" + level.zombies[i].spriteFacing + "_" + level.zombies[i].spriteNumber;
                send_draw_info(startX + level.zombies[i].sX, startY + level.zombies[i].sY, spriteInfo, 1.0f * pauseDim, s_zombie);
            }
        }

        // Liches
        for (int i = 0; i < level.liches.size(); i++) {
            if (level.liches[i].gY == y && level.liches[i].visible) {
                std::string spriteInfo = "lich_" + level.liches[i].type + "_" + level.liches[i].currentAction + "_" + level.liches[i].spriteFacing + "_" + level.liches[i].spriteNumber;
                send_draw_info(startX + level.liches[i].sX, startY + level.liches[i].sY, spriteInfo, 1.0f * pauseDim, s_lich);
            }
        }

        // Wraiths
        for (int i = 0; i < level.wraiths.size(); i++) {
            if (level.wraiths[i].gY == y && level.wraiths[i].visible) {
                std::string spriteInfo = "wraith_" + level.wraiths[i].type + "_" + level.wraiths[i].currentAction + "_" + level.wraiths[i].spriteFacing + "_" + level.wraiths[i].spriteNumber;
                send_draw_info(startX + level.wraiths[i].sX, startY + level.wraiths[i].sY, spriteInfo, 1.0f * pauseDim, s_wraith);
            }
        }
        */
        // Entities (lower half of a tile)
        for (int i = 0; i < level.entities.size(); i++) {
            if (level.entities[i]->gPos.y == y) {
                int cellY = level.entities[i]->cPos.y - (y * cellSize);

                if (cellY >= cellHeight / 2 && level.entities[i]->visible) {
                    //send_draw_info_IRECT(startPos + level.entities[i]->sPos, 1.0f, sprites[level.entities[i]->spriteIndex], level.entities[i]->spriteRegion);
                    gfx.DrawSprite(startPos + level.entities[i]->sPos, sprites[level.entities[i]->spriteIndex]);
                }
            }
        }

        sdd.brightness = 1.0f;
        sdd.ira = true;
        // Animations
        for (int i = 0; i < level.animations.size(); i++) {
            if (!level.animations[i].ground) {
                if ((level.animations[i].cPos / cellSize).y == y) {
                    sdd.irect = level.animations[i].spriteRegion;

                    gfx.DrawSprite(startPos + level.animations[i].sPos, sprites[level.animations[i].spriteIndex], sdd);
                }
            }
        }
    }

    // Particles (non-text)
    {
        SpriteDrawData sdd;

        for (int i = 0; i < level.particles.size(); i++) {
            if (!(level.particles[i].dtype == PARTICLE_TEXT)) {
                sdd.transparency = level.particles[i].transparency;

                if (level.particles[i].dtype == PARTICLE_SPRITE) {
                    gfx.DrawSprite(startPos + level.particles[i].sPos, sprites[level.particles[i].spriteIndex], sdd);
                }
                else if (level.particles[i].dtype == PARTICLE_RECTANGLE) {
                    gfx.DrawSprite(startPos + level.particles[i].sPos, level.particles[i].srect, sdd);
                }
            }
        }
    }

    /*
/// Draw enemy healthbars
    // Shooters
    for (int i = 0; i < level.shooters.size(); i++) {
        if (level.shooters[i].visible && level.shooters[i].healthbarVisible) {
            // Health
            draw_enemy_health(startX, startY, level.shooters[i].cX, level.shooters[i].cY - level.shooters[i].healthbarHeight, level.shooters[i].health, level.shooters[i].maxHealth, pauseDim);
            // Armor
            draw_enemy_armor(startX, startY, level.shooters[i].cX, level.shooters[i].cY - level.shooters[i].healthbarHeight, level.shooters[i].armor, pauseDim);
        }
    }

    // Barbarians
    for (int i = 0; i < level.barbarians.size(); i++) {
        if (level.barbarians[i].visible && level.barbarians[i].healthbarVisible) {
            // Health
            draw_enemy_health(startX, startY, level.barbarians[i].cX, level.barbarians[i].cY - level.barbarians[i].healthbarHeight, level.barbarians[i].health, level.barbarians[i].maxHealth, pauseDim);
            // Armor
            draw_enemy_armor(startX, startY, level.barbarians[i].cX, level.barbarians[i].cY - level.barbarians[i].healthbarHeight, level.barbarians[i].armor, pauseDim);
        }
    }

    // Wizards
    for (int i = 0; i < level.wizards.size(); i++) {
        if (level.wizards[i].visible && level.wizards[i].healthbarVisible) {
            // Health
            draw_enemy_health(startX, startY, level.wizards[i].cX, level.wizards[i].cY - level.wizards[i].healthbarHeight, level.wizards[i].health, level.wizards[i].maxHealth, pauseDim);
            // Armor
            draw_enemy_armor(startX, startY, level.wizards[i].cX, level.wizards[i].cY - level.wizards[i].healthbarHeight, level.wizards[i].armor, pauseDim);
        }
    }

    // Paladins
    for (int i = 0; i < level.paladins.size(); i++) {
        if (level.paladins[i].visible && level.paladins[i].healthbarVisible) {
            // Health
            draw_enemy_health(startX, startY, level.paladins[i].cX, level.paladins[i].cY - level.paladins[i].healthbarHeight, level.paladins[i].health, level.paladins[i].maxHealth, pauseDim);
            // Armor
            draw_enemy_armor(startX, startY, level.paladins[i].cX, level.paladins[i].cY - level.paladins[i].healthbarHeight, level.paladins[i].armor, pauseDim);
        }
    }

    // Zombies
    for (int i = 0; i < level.zombies.size(); i++) {
        if (level.zombies[i].visible && level.zombies[i].healthbarVisible) {
            // Health
            draw_enemy_health(startX, startY, level.zombies[i].cX, level.zombies[i].cY - level.zombies[i].healthbarHeight, level.zombies[i].health, level.zombies[i].maxHealth, pauseDim);
            // Armor
            draw_enemy_armor(startX, startY, level.zombies[i].cX, level.zombies[i].cY - level.zombies[i].healthbarHeight, level.zombies[i].armor, pauseDim);
        }
    }

    // Liches
    for (int i = 0; i < level.liches.size(); i++) {
        if (level.liches[i].visible && level.liches[i].healthbarVisible) {
            // Health
            draw_enemy_health(startX, startY, level.liches[i].cX, level.liches[i].cY - level.liches[i].healthbarHeight, level.liches[i].health, level.liches[i].maxHealth, pauseDim);
            // Armor
            draw_enemy_armor(startX, startY, level.liches[i].cX, level.liches[i].cY - level.liches[i].healthbarHeight, level.liches[i].armor, pauseDim);
        }
    }

/// Draw Text
    // Sign text
    draw_sign_text(startX, startY, pauseDim);
    */
    // Text particles
    {
        TextRenderData txr;
        txr.centered = true;

        for (int i = 0; i < level.particles.size(); i++) {
            if (level.particles[i].dtype == PARTICLE_TEXT) {
                txr.transparency = level.particles[i].transparency;
                render_text(startPos + level.particles[i].cPos, level.particles[i].text, txr);
            }
        }
    }

    //draw_text(50, 500, "! #$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ", false, false, 1.0f);
    //draw_text(50, 524, "[\\]^_`abcdefghijklmopqrstuvwxyz{|}~", false, false, 1.0f);

/// Draw UI
    // Health
    draw_player_health();
    // Armor
    draw_player_armor();
    // Items
    draw_player_items();
    // Gold
    draw_player_gold();
    // Effects
    /// draw_player_effects(startX, startY, pauseDim);
    // Minimap
    draw_minimap();

    // Screen animation
    screenAnimations.apply_slow(gfx.pSysBuffer);

    // Draw pause and console
    {
        TextRenderData txr;
        txr.centered = true;
        txr.font = "basic-large";

        if (level.paused) {
            gfx.ChangeBrightness(0.5f);
            render_text(centerPos + Pos2D(0, -12), "paused", txr);
        }

        txr.centered = false;
        txr.font = "basic-small";
        txr.size = 2;

        if (level.console.is_opened()) {
            gfx.ChangeBrightness(0.75f);
            render_text(Pos2D(4, 4), level.console.get_text(), txr);
        }
    }

    /*
    // Draw end screen
    if (level.inEndScreen && !level.startedEndEffect) {
        screenAnimations.start_end_effect();
        level.startedEndEffect = true;
        return;
    }
    else if (level.inEndScreen) {
        draw_end_screen(centerX, centerY);
        return;
    }
    */

    // Apply level start fade
    if (level.inFA) {
        gfx.ChangeBrightness(level.FAprogress * level.FAprogress);
    }
}

void Game::draw_enemy_health(int startX, int startY, int hstartX, int hstartY, int health, int maxHealth)
{
    /*
    int healthToDraw = health;

    int drawX = startX + hstartX - (((maxHealth / 2 * 24) - 2) / 2);
    int drawY = startY + hstartY - 22;
    for (int i = 0; i < maxHealth / 2; i++) {
        if (healthToDraw >= 2) {
            send_draw_info(drawX, drawY, "heart_small_full", 1.0f, s_ui);
            healthToDraw -= 2;
        }
        else if (healthToDraw == 1) {
            send_draw_info(drawX, drawY, "heart_small_half", 1.0f, s_ui);
            healthToDraw -= 1;
        }
        else if (healthToDraw == 0) {
            send_draw_info(drawX, drawY, "heart_small_empty", 1.0f, s_ui);
        }

        drawX += 24;
    }
    */
}

void Game::draw_enemy_armor(int startX, int startY, int astartX, int astartY, int armor)
{
    /*
    int armorToDraw = armor;

    int drawX = startX + astartX - (((ceil((float)armor / 2) * 24) - 2) / 2);
    int drawY = startY + astartY;
    while (1) {
        if (armorToDraw >= 2) {
            send_draw_info(drawX, drawY, "shield_small_full", 1.0f, s_ui);
            armorToDraw -= 2;
        }
        else if (armorToDraw == 1) {
            send_draw_info(drawX, drawY, "shield_small_half", 1.0f, s_ui);
            armorToDraw -= 1;
        }
        else {
            break;
        }

        drawX += 24;
    }
    */
}

void Game::draw_player_health()
{
    int hstartX = 50;
    int hstartY = 50;

    int rows = ceil((float)level.players[0].maxHealth / 10);
    int healthToDraw = level.players[0].health;

    int rowHeight = 50 - (rows - 1) * 4;
    if (rowHeight < 10) rowHeight = 10;

    int drawX = hstartX;
    int drawY = hstartY;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < 5; j++) {
            if (i == rows - 1) {
                if (j == (level.players[0].maxHealth / 2 % 5) && j != 0) {
                    break;
                }
            }
            if (healthToDraw >= 2) {
                send_draw_info(Pos2D(drawX, drawY), 1.0f, sprites[HEART_FULL]);
                healthToDraw -= 2;
            }
            else if (healthToDraw == 1) {
                send_draw_info(Pos2D(drawX, drawY), 1.0f, sprites[HEART_HALF]);
                healthToDraw -= 1;
            }
            else if (healthToDraw == 0) {
                send_draw_info(Pos2D(drawX, drawY), 1.0f, sprites[HEART_EMPTY]);
            }

            drawX += 56;
        }

        drawX = hstartX;
        drawY += rowHeight;
    }
}

void Game::draw_player_armor()
{
    int astartX = 386;
    int astartY = 50;

    int rows = ceil((float)level.players[0].armor / 10);
    int armorToDraw = level.players[0].armor;

    int rowHeight = 50 - (rows - 1) * 4;
    if (rowHeight < 10) rowHeight = 10;

    int drawX = astartX;
    int drawY = astartY;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < 5; j++) {
            if (armorToDraw >= 2) {
                send_draw_info(Pos2D(drawX, drawY), 1.0f, sprites[SHIELD_FULL]);
                armorToDraw -= 2;
            }
            else if (armorToDraw == 1) {
                send_draw_info(Pos2D(drawX, drawY), 1.0f, sprites[SHIELD_HALF]);
                armorToDraw -= 1;
            }
            else if (armorToDraw == 0) {
                break;
            }

            drawX += 56;
        }

        drawX = astartX;
        drawY += rowHeight;
    }
}

void Game::draw_player_items()
{
    /*
    int startX = gfx.ScreenWidth - 499;
    int startY = 50;

    int drawX = startX;
    int drawY = startY;

    // Draw items
    for (int i = 0; i < 5; i++) {
        if (i < level.player[0].items.size()) {
            send_draw_info(drawX + 8, drawY + 8, level.player[0].items[i].spriteName, 1.0f * pauseDim, s_items);
        }

        if (i == level.player[0].currentlySelectedItem) {
            send_draw_info(drawX, drawY, "item_frame_selected", 1.0f * pauseDim, s_ui);
        }
        else {
            send_draw_info(drawX, drawY, "item_frame", 1.0f * pauseDim, s_ui);
        }

        // Draw cooldown bar
        {
            duration<float> time_elapsed = system_clock::now() - level.player[0].items[i].lastUsed;
            if (time_elapsed < milliseconds(level.player[0].items[i].cooldown)) {
                float cd = level.player[0].items[i].cooldown / 1000.0f;
                float cooldownProgress = time_elapsed.count() / cd;

                int barLength = round(cooldownProgress * 14) * 4;

                Sprite cdBar = create_rectangle(barLength, 4, Color(0, 255, 0));

                send_draw_info(drawX + 12, drawY + 68, 1.0f * pauseDim, cdBar);
            }
        }

        if (i == 0) {
            draw_text(drawX, drawY + 68, "1", false, false, 1.0f);
        }
        else if (i == 1) {
            draw_text(drawX - 4, drawY + 68, "2", false, false, 1.0f);
        }
        else if (i == 2) {
            draw_text(drawX - 4, drawY + 68, "3", false, false, 1.0f);
        }
        else if (i == 3) {
            draw_text(drawX - 4, drawY + 68, "4", false, false, 1.0f);
        }
        else if (i == 4) {
            draw_text(drawX - 4, drawY + 68, "5", false, false, 1.0f);
        }

        drawX += 92;
    }
    */
}

void Game::draw_player_gold()
{
    Pos2D startPos(50, gfx.ScreenHeight - 70);

    send_draw_info(startPos, 1.0f, sprites[GOLD_COIN]);

    std::ostringstream ss;
    ss << level.players[0].gold;
    render_text(startPos + Pos2D(24, 0), ss.str());
}

void Game::draw_player_effects(int startX, int startY)
{
    /*
    if (level.player[0].statusEffects.slowed) {
        send_draw_info(startX + level.player[0].sX - 40, startY + level.player[0].sY - 28, "effect_slowed", 1.0f * pauseDim, s_other);
    }
    if (level.player[0].statusEffects.stunned) {
        send_draw_info(startX + level.player[0].sX + 8, startY + level.player[0].sY - 28, "effect_stunned", 1.0f * pauseDim, s_other);
    }
    if (level.player[0].statusEffects.burning) {
        send_draw_info(startX + level.player[0].sX + 56, startY + level.player[0].sY - 28, "effect_burning", 1.0f * pauseDim, s_other);
    }
    */
}

void Game::draw_minimap()
{
    /*
    if (level.minimap.get_size() == 1) {
        int startX = gfx.ScreenWidth - 200;
        int startY = gfx.ScreenHeight - 200;

        int    ox = 0;
        int oy = 0;

        for (int y = level.player[0].gY - 10; y <= level.player[0].gY + 10; y++) {
            ox = 0;

            for (int x = level.player[0].gX - 10; x <= level.player[0].gX + 10; x++) {
                int index = y * level.width + x;

                if (x >= 0 && x < level.width && y >= 0 && y < level.height) {
                    send_draw_info(startX + ox * 8, startY + oy * 8, 1.0f * pauseDim, level.minimap.cells[level.minimap.grid[index]]);
                }

                ox++;
            }

            oy++;
        }
    }
    else if (level.minimap.get_size() == 2) {
        int startX = gfx.ScreenWidth / 2 - level.width * 8 / 2;
        int startY = gfx.ScreenHeight / 2 - level.height * 8 / 2;

        for (int y = 0; y < level.height; y++) {
            for (int x = 0; x < level.width; x++) {
                int index = y * level.width + x;

                send_draw_info(startX + x * 8, startY + y * 8, 1.0f * pauseDim, level.minimap.cells[level.minimap.grid[index]], 0.5f);
            }
        }
    }
    */
}

void Game::draw_death_screen(int centerX, int centerY)
{
    /*
    duration<float> elapsed_seconds = system_clock::now() - level.deathScreenStart;

    float DStimeElapsed = elapsed_seconds.count();

    int textOffsetY;
    if (DStimeElapsed >= 0.5f) {
        float DSTAprogress = (DStimeElapsed - 0.5f) / 0.5f;

        if (DSTAprogress > 1.0f) DSTAprogress = 1.0f;

        textOffsetY = -userData.cWindowHeight * pow(1.0f - DSTAprogress, 3);

        draw_text(Pos2D(centerX, centerY + textOffsetY - 60), "you died", true, true, 1.0f);
        draw_text(Pos2D(centerX, centerY + textOffsetY + 10), "press \"r\" to restart from last checkpoint", true, false, 1.0f);
        draw_text(Pos2D(centerX, centerY + textOffsetY + 50), "press \"enter\" to exit", true, false, 1.0f);

        if (level.inEDFA) {
            if (level.EDFAprogress >= 1.0f) {
                if        (level.newMode == "game") {
                    inGame                = true;
                    level.inEDFA        = false;
                    level.inDeathScreen = false;

                    level.load_save_file("test.txt");

                    gfx.ChangeBrightness(1.0f - sqrt(level.EDFAprogress));
                    return;
                }
                else if (level.newMode == "menu") {
                    inGame                = false;
                    levelLoaded            = false;
                    level.inEDFA        = false;
                    level.inDeathScreen = false;

                    menu.enter_menu(0, 0);

                    gfx.ChangeBrightness(1.0f - sqrt(level.EDFAprogress));
                    return;
                }
            }
            
            gfx.ChangeBrightness(1.0f - sqrt(level.EDFAprogress));
        }
        else {
            if (keys.key_state(keys.key_code("r"))) {
                level.inEDFA    = true;
                level.EDFAstart = system_clock::now();
                level.newMode    = "game";
            }
            if (keys.key_state(keys.key_code("enter"))) {
                level.inEDFA    = true;
                level.EDFAstart = system_clock::now();
                level.newMode    = "menu";
            }
        }
    }
    else {
        screenAnimations.apply_death_effect(gfx.pSysBuffer);
    }
    */
}

void Game::draw_end_screen(int centerX, int centerY)
{
    /*
    screenAnimations.apply_end_effect(gfx.pSysBuffer);

    duration<float> elapsed_seconds = system_clock::now() - level.endScreenStart;

    float EStimeElapsed = elapsed_seconds.count();

    int textOffsetY;
    if (EStimeElapsed >= 1.0f) {
        float ESTAprogress = (EStimeElapsed - 1.0f) / 0.5f;

        if (ESTAprogress > 1.0f) ESTAprogress = 1.0f;

        textOffsetY = -userData.cWindowHeight * pow(1.0f - ESTAprogress, 3);

        draw_text(Pos2D(centerX, centerY + textOffsetY - 60), "level completed", true, true, 1.0f);
        draw_text(Pos2D(centerX, centerY + textOffsetY + 10), "Time - " + level.levelTime, true, false, 1.0f);
        draw_text(Pos2D(centerX, centerY + textOffsetY + 50), "press \"enter\" to exit", true, false, 1.0f);

        if (level.inEDFA) {
            if (level.EDFAprogress >= 1.0f) {
                if (level.newMode == "menu") {
                    inGame              = false;
                    levelLoaded          = false;
                    level.inEDFA      = false;
                    level.inEndScreen = false;

                    menu.enter_menu(0, 0);

                    gfx.ChangeBrightness(1.0f - sqrt(level.EDFAprogress));
                    return;
                }
            }

            gfx.ChangeBrightness(1.0f - sqrt(level.EDFAprogress));
        }
        else {
            if (keys.key_state(keys.key_code("enter"))) {
                level.inEDFA    = true;
                level.EDFAstart = system_clock::now();
                level.newMode    = "menu";
            }
        }
    }
    */
}

void Game::draw_sign_text(int x, int y)
{
    for (int i = 0; i < level.signs.size(); i++) {
        if (level.signs[i].playerNearby) {
            int startX = level.signs[i].gX * cellSize + cellSize / 2;
            int startY = level.signs[i].gY * cellSize + cellSize / 2 - 100;

            startY -= (level.signs[i].textLine.size() - 1) * 24;

            for (int j = 0; j < level.signs[i].textLine.size(); j++) {
                //draw_text(Pos2D(x + startX, y + startY), level.signs[i].textLine[j], true, false, 1.0f);

                startY += 24;
            }
        }
    }
}

void Game::apply_shake(int &startX, int &startY)
{
    screenAnimations.apply_shake(startX, startY);
}

void Game::unpause()
{
    /*
    if (keys.key_state(userData.keyBindings.PAUSE)) {
        auto pauseDuration = std::chrono::system_clock::now() - level.pauseStartTime;

        // Level
        level.levelStartTime += pauseDuration;

        // Player
        level.player[0].MAstart += pauseDuration;

        // Entities
        for (int i = 0; i < level.entities.size(); i++) {
            level.entities[i].creationTime      += pauseDuration;
            level.entities[i].lastMove          += pauseDuration;
            level.entities[i].lastFrameChange += pauseDuration;
        }

        // Shooters
        for (int i = 0; i < level.shooters.size(); i++) {
            level.shooters[i].creationTime += pauseDuration;
            level.shooters[i].lastShotTime += pauseDuration;
        }

        // Barbarians
        for (int i = 0; i < level.barbarians.size(); i++) {
            level.barbarians[i].lastTookDamage    += pauseDuration;
            level.barbarians[i].lastAction        += pauseDuration;
            level.barbarians[i].MAstart            += pauseDuration;
            level.barbarians[i].AAstart            += pauseDuration;
        }

        // Wizards
        for (int i = 0; i < level.wizards.size(); i++) {
            level.wizards[i].lastTookDamage += pauseDuration;
            level.wizards[i].lastAction        += pauseDuration;
            level.wizards[i].MAstart        += pauseDuration;
            level.wizards[i].BAstart        += pauseDuration;
            level.wizards[i].BAlastCast        += pauseDuration;
            level.wizards[i].EAstart        += pauseDuration;
            level.wizards[i].EAlastCast        += pauseDuration;
            level.wizards[i].SAstart        += pauseDuration;
            level.wizards[i].SAlastCast        += pauseDuration;
        }

        // Paladins
        for (int i = 0; i < level.wizards.size(); i++) {
            level.paladins[i].lastTookDamage += pauseDuration;
            level.paladins[i].lastAction     += pauseDuration;
            level.paladins[i].MAstart         += pauseDuration;
            level.paladins[i].AAstart         += pauseDuration;
            level.paladins[i].CAstart         += pauseDuration;
            level.paladins[i].lastCharge     += pauseDuration;
        }

        // Zombies
        for (int i = 0; i < level.zombies.size(); i++) {
            level.zombies[i].lastTookDamage += pauseDuration;
            level.zombies[i].lastAction        += pauseDuration;
            level.zombies[i].MAstart        += pauseDuration;
            level.zombies[i].AAstart        += pauseDuration;
        }

        // Liches
        for (int i = 0; i < level.liches.size(); i++) {
            level.liches[i].lastTookDamage    += pauseDuration;
            level.liches[i].lastAction        += pauseDuration;
            level.liches[i].MAstart            += pauseDuration;
            level.liches[i].BAstart            += pauseDuration;
            level.liches[i].SAstart            += pauseDuration;
        }

        // Wraiths
        for (int i = 0; i < level.wraiths.size(); i++) {
            level.wraiths[i].lastTookDamage += pauseDuration;
            level.wraiths[i].lastAction        += pauseDuration;
            level.wraiths[i].MAstart        += pauseDuration;
        }

        // Particles
        for (int i = 0; i < level.particles.size(); i++) {
            level.particles[i].creationTime += pauseDuration;
            level.particles[i].lastUpdated    += pauseDuration;
        }

        // Visual effects
        for (int i = 0; i < level.visualEffects.size(); i++) {
            level.visualEffects[i].creationTime += pauseDuration;
        }

        level.paused = false;
    }
    */
}
