/****************************************************************************************** 
 *    Chili DirectX Framework Version 16.07.20                                              *    
 *    Game.h                                                                                  *
 *    Copyright 2016 PlanetChili.net <http://www.planetchili.net>                              *
 *                                                                                          *
 *    This file is part of The Chili DirectX Framework.                                      *
 *                                                                                          *
 *    The Chili DirectX Framework is free software: you can redistribute it and/or modify      *
 *    it under the terms of the GNU General Public License as published by                  *
 *    the Free Software Foundation, either version 3 of the License, or                      *
 *    (at your option) any later version.                                                      *
 *                                                                                          *
 *    The Chili DirectX Framework is distributed in the hope that it will be useful,          *
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of                          *
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                          *
 *    GNU General Public License for more details.                                          *
 *                                                                                          *
 *    You should have received a copy of the GNU General Public License                      *
 *    along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#pragma once

// Global stuff/constants/helper headers
#include "Constants.h"
#include "HelperStructs.h"
#include "Sol.hpp"

// Messages
#include "GameMessage.h"

// Game components
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Textures.h"
#include "Menu.h"
#include "Level.h"
#include "Sprite.h"
#include "ScreenAnimations.h"
#include "Font.h"

// Key bindings/keyboard
#include "Keys.h"
#include "KeyBindings.h"

// Loaded data
#include "LevelData.h"
#include "UserData.h"

// C++ stuff
#include <string>
#include <vector>

using namespace std::chrono;

class Game
{
public:
    Game( class MainWindow& wnd );
    Game( const Game& ) = delete;
    Game& operator=( const Game& ) = delete;
    void Go();
    void load_config();
    void load_menu();
    void read_save_file_names();
    void read_user_level_names();
    void read_textures();
    void load_items();
    void load_objects();
    int load_sprite(std::string fileDir);
    //void ReadLevels();
private:
    void ComposeFrame();
    void ComposeMenu();
    void ComposeGame();
    void UpdateModel();
private:
    MainWindow& wnd;
    Graphics gfx;
public:
    // Variables
    kb::Keys keys;

    UserData userData;

    int levelCount;

    //std::vector<Sprite> sprites;
    /*
    std::vector<Sprite> s_tiles;
    std::vector<Sprite> s_entities;
    std::vector<Sprite> s_font_large;
    std::vector<Sprite> s_items;
    std::vector<Sprite> s_player;
    std::vector<Sprite> s_wiring;
    std::vector<Sprite> s_doors;
    std::vector<Sprite> s_ui;
    std::vector<Sprite> s_menu;
    std::vector<Sprite> s_other;

    std::vector<Sprite> s_shooter;
    std::vector<Sprite> s_barbarian;
    std::vector<Sprite> s_wizard;
    std::vector<Sprite> s_paladin;
    std::vector<Sprite> s_zombie;
    std::vector<Sprite> s_lich;
    std::vector<Sprite> s_wraith;

    std::vector<Sprite> s_vfx;

    std::vector<Sprite> s_ptc_general;
    */

    std::vector<LevelData> levelData;

    Font f_small;
    Font f_large;

    Menu menu;
    Level level;

    ScreenAnimations screenAnimations;

    time_point<system_clock> programStart;

    // Struct of variables, used to enter/load a game state
    GameStateData gameStateData;

    // Game messages
    std::vector<GameMessage*> gameMessages;

    // FPS counter
    TimePoint frameStartTime;

    // One time use
    bool readFailed;

    // Screenshots
    std::vector<Sprite> screenshots;

/// Functions
    // Level
    void draw_enemy_health(int startX, int startY, int hstartX, int hstartY, int health, int maxHealth);
    void draw_enemy_armor(int startX, int startY, int astartX, int astartY, int armor);
    void draw_player_health();
    void draw_player_armor();
    void draw_player_items();
    void draw_player_gold();
    void draw_player_effects(int startX, int startY);
    void draw_minimap();

    void draw_death_screen(int centerX, int centerY);
    void draw_end_screen(int centerX, int centerY);

    void draw_sign_text(int x, int y);

    void apply_shake(int &startX, int &startY);

    void unpause();

    // Menu
    void draw_elements(int centerX, float menuBrightness);

    // Main
    void send_draw_info       (Pos2D pos, float brightness, const Sprite &spr,                   float transparency = 1.0f);
    void send_draw_info_IRECT (Pos2D pos, float brightness, const Sprite &spr, RECT ri,          float transparency = 1.0f);
    void send_draw_info_ORECT (Pos2D pos, float brightness, const Sprite &spr,          RECT ro, float transparency = 1.0f);
    void send_draw_info_IORECT(Pos2D pos, float brightness, const Sprite &spr, RECT ri, RECT ro, float transparency = 1.0f);
    //void send_draw_info(Pos2D pos, std::string sprname, float brightness, std::vector<Sprite> &spr, float transparency = 0.0f);
    //void send_draw_info(int x, int y, float brightness, Sprite spr, RECT r, float transparency = 0.0f);
    //void send_draw_info(int x, int y, std::string sprname, float brightness, RECT r, std::vector<Sprite> &spr, float transparency = 0.0f);
    void draw_text(Pos2D pos, std::string text, bool centered, bool large, float brightness, float transparency = 1.0f);
};
