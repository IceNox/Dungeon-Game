#include "Game.h"

#include "GlobalData.h"

#include "Functions.h"
#include "GameTime.h"

#include "Sol.hpp"

#include "MainWindow.h"

#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>

using namespace std::chrono;

int PXL_RATIO = 1;

std::vector<std::string> _SAVE_FILE_NAMES;
std::vector<std::string> _USER_LEVEL_FILE_NAMES;

std::vector<Sprite> sprites;
std::vector<std::string> spriteFilePaths;

std::vector<ScriptLoadData> _ITEM_SCRIPTS;
std::vector<ScriptLoadData> _OBJECT_SCRIPTS;

Game::Game(MainWindow& wnd)
    :
    wnd(wnd)
{
    // Start game timer
    maintime::initialize();

    // Read levels/saves
    read_save_file_names();

    // Load settings, menu, textures
    load_config();
    load_menu();
    read_textures();
    //ReadLevels();
    //load_items();
    load_objects();


    //screenAnimations.set_window_parameters(userData.cWindowWidth, userData.cWindowHeight);

    // Set font variables
    f_small.set_variables("small");
    f_large.set_variables("large");

    programStart = std::chrono::system_clock::now();

    // Create window
    gfx.Initialize
    (
        wnd,
        userData.settings.fullscreen,
        userData.cWindowWidth,
        userData.cWindowHeight
    );
}

void Game::Go()
{
    // Update game time
    maintime::update_time();

    // Start Frame timer
    frameStartTime = maintime::now();

    // Create frame
    gfx.BeginFrame();
    UpdateModel();
    ComposeFrame();
    gfx.EndFrame();
}

void Game::UpdateModel()
{
    // Handle game messages
    for (auto it : gameMessages) {
        it->handle_msg(gameStateData);
    }

    gameMessages.clear();

    // Update key states
    keys.update_key_states();

    // Update level model
    if (gameStateData.inLevel) {
        if (gameStateData.enterLevel) {
            // Load level if nescessary
            level.load_game(gameStateData.lType, gameStateData.lName);
            level.update_level(gameMessages, screenAnimations, keys, userData);
            gameStateData.levelLoaded = true;
            gameStateData.enterLevel  = false;
        }
        else if (!gameStateData.levelLoaded) {
            // If level isn't loaded go back to menu
            menu.enter_menu(0, 0);
            gameStateData.inLevel = false;
        }
        else {
            // Don't update the model if game is paused
            if (!level.paused) {
                level.update_level(gameMessages, screenAnimations, keys, userData);
            }
            else {
                unpause();
            }
        }
    }
    // Update editor model
    else if (gameStateData.inEditor) {

    }
    // Update menu model
    else {
        menu.update_menu(gameMessages, keys, userData);
        gameStateData.levelLoaded = false;
    }
}

void Game::ComposeFrame()
{
    // Update 4k mode scaling incase it was changed
    if (userData.settings.mode4k)
        PXL_RATIO = 2;
    else
        PXL_RATIO = 1;

    // Compose frame
    if        (gameStateData.inLevel) {
        ComposeGame();
    }
    else if (gameStateData.inEditor) {

    }
    else {
        ComposeMenu();
    }

    // Update time again
    maintime::update_time();

    // Show frame counter
    int time = (maintime::now() - frameStartTime).get_duration();
    int FPS = round(1000000.0 / time);

    std::ostringstream ss;
    ss << FPS;
    draw_text(Pos2D(4, gfx.ScreenHeight - 24), ss.str(), false, false, 1.0f);
    
    // Show test timer
    std::ostringstream ss2;
    ss2 << maintime::now().get_time(SECONDS);
    draw_text(Pos2D(4, gfx.ScreenHeight - 48), ss2.str(), false, false, 1.0f);
}

void Game::send_draw_info        (Pos2D pos, float brightness, Sprite spr,                    float transparency)
{
    gfx.DrawSprite(pos, spr, brightness, transparency);
}

void Game::send_draw_info_IRECT    (Pos2D pos, float brightness, Sprite spr, RECT ri,            float transparency)
{
    gfx.DrawSpriteRECT(pos, spr, brightness, ri, { 0, 0, gfx.ScreenWidth, gfx.ScreenHeight }, transparency);
}

void Game::send_draw_info_ORECT    (Pos2D pos, float brightness, Sprite spr,           RECT ro,    float transparency)
{
    gfx.DrawSpriteRECT(pos, spr, brightness, { 0, 0, spr.GetWidth(), spr.GetHeight() }, ro, transparency);
}

void Game::send_draw_info_IORECT(Pos2D pos, float brightness, Sprite spr, RECT ri, RECT ro, float transparency)
{
    gfx.DrawSpriteRECT(pos, spr, brightness, ri, ro, transparency);
}
/*
void Game::send_draw_info(Pos2D pos, std::string sprname, float brightness, std::vector<Sprite> &spr, float transparency)
{
    for (int i = 0; i < spr.size(); i++) {
        if (spr[i].GetName() == sprname) {
            if (transparency == 0.0f) {
                gfx.DrawSprite(pos.x, pos.y, spr[i], brightness);
            }
            else {
                gfx.DrawSpriteTransparent(pos.x, pos.y, spr[i], brightness, transparency);
            }
            break;
        }
    }
}

void Game::send_draw_info(int x, int y, float brightness, Sprite spr, RECT r, float transparency)
{
    if (transparency == 0.0f) {
        gfx.DrawSpriteRECT(x, y, spr, brightness, r);
    }
    else {
        gfx.DrawSpriteTransparentRECT(x, y, spr, brightness, transparency, r);
    }
}

void Game::send_draw_info(int x, int y, std::string sprname, float brightness, RECT r, std::vector<Sprite> &spr, float transparency)
{
    for (int i = 0; i < spr.size(); i++) {
        if (spr[i].GetName() == sprname) {
            if (transparency == 0.0f) {
                gfx.DrawSpriteRECT(x, y, spr[i], brightness, r);
            }
            else {
                gfx.DrawSpriteTransparentRECT(x, y, spr[i], brightness, transparency, r);
            }
            break;
        }
    }
}
*/
void Game::draw_text(Pos2D pos, std::string text, bool centered, bool large, float brightness, float transparency)
{
    if (large) {
        /// Large text drawing

        // Set starting x coordinate
        int startX = pos.x;

        // If text is centered, evaluate new starting x coordinate
        if (centered) {
            int textWidth = text.length() * 40 - 8;
            startX = pos.x - textWidth / 2;
        }

        // Draw every letter
        int textPos = 0;
        for (int i = 0; i < text.length(); i++) {
            send_draw_info(Pos2D(startX + textPos, pos.y), brightness, f_large.character[(int)text[i] - 32], transparency);

            // Set next letter pos
            textPos += f_large.characterW[(int)text[i] - 32];
            textPos += 8;
        }
    }
    else {
        /// Small text drawing

        // Set starting x coordinate
        int startX = pos.x;

        // If text is centered, evaluate new starting x coordinate
        if (centered) {
            int textWidth = 0;
            for (int i = 0; i < text.length(); i++) {
                textWidth += f_small.characterW[(int)text[i] - 32];

                textWidth += 4;
            }
            textWidth -= 4;

            startX = pos.x - textWidth / 2;
        }

        // Draw every letter
        int textPos = 0;
        for (int i = 0; i < text.length(); i++) {
            send_draw_info(Pos2D(startX + textPos, pos.y), brightness, f_small.character[(int)text[i] - 32], transparency);

            // Set next letter pos
            textPos += f_small.characterW[(int)text[i] - 32];
            textPos += 4;
        }
    }
}

void Game::load_config()
{
    sol::state config;
    config.script_file("Content/User/config2.lua");

    /// Settings
    sol::table settings = config["settings"];
    userData.settings.fullscreen    = false;//settings["fullscreen"];
    userData.settings.window_width    = settings["resolution"]["width"];
    userData.settings.window_height = settings["resolution"]["height"];
    userData.settings.native        = settings["native"];
    userData.settings.mode4k        = settings["mode4k"];
    
    if (userData.settings.fullscreen && userData.settings.native) {
        userData.cWindowWidth  = userData.screenWidth;
        userData.cWindowHeight = userData.screenHeight;
    }
    else {
        userData.cWindowWidth  = userData.settings.window_width;
        userData.cWindowHeight = userData.settings.window_height;
    }

    /// Keybinds
    sol::table keybinds = config["keybinds"];
    userData.keyBindings.MOVE_UP            = keybinds.get<int>("moveup");
    userData.keyBindings.MOVE_DOWN            = keybinds.get<int>("movedown");
    userData.keyBindings.MOVE_LEFT            = keybinds.get<int>("moveleft");
    userData.keyBindings.MOVE_RIGHT            = keybinds.get<int>("moveright");
    userData.keyBindings.CHANGE_DIRECTION    = keybinds.get<int>("changedirection");
    userData.keyBindings.CHANGE_MAP_SIZE    = keybinds.get<int>("changemapsize");
    userData.keyBindings.USE_ITEM            = keybinds.get<int>("useitem");
    userData.keyBindings.DROP_ITEM            = keybinds.get<int>("dropitem");
    userData.keyBindings.ITEM_1                = keybinds.get<int>("item1");
    userData.keyBindings.ITEM_2                = keybinds.get<int>("item2");
    userData.keyBindings.ITEM_3                = keybinds.get<int>("item3");
    userData.keyBindings.ITEM_4                = keybinds.get<int>("item4");
    userData.keyBindings.ITEM_5                = keybinds.get<int>("item5");
    userData.keyBindings.PAUSE                = keybinds.get<int>("pause");

    return;

    std::ifstream in;
    in.open("Content/User/config.txt");

    std::string typeName;
    std::string t;
    int code;

    readFailed = false;

    /// Settings
    in >> typeName;

    // Screen
    in >> t;
    bool f; in >> f;
    if (!in.fail()) userData.settings.fullscreen = f;
    else readFailed = true;

    in >> t;
    int w; in >> w;
    if (!in.fail()) userData.settings.window_width = w;
    else readFailed = true;

    int h; in >> h;
    if (!in.fail()) userData.settings.window_height = h;
    else readFailed = true;

    in >> t;
    bool n; in >> n;
    if (!in.fail()) userData.settings.native = n;
    else readFailed = true;

    in >> t;
    bool k4; in >> k4;
    if (!in.fail()) userData.settings.mode4k = k4;
    else readFailed = true;


    if (f && n) {
        userData.cWindowWidth = userData.screenWidth;
        userData.cWindowHeight = userData.screenHeight;
    }
    else {
        userData.cWindowWidth = userData.settings.window_width;
        userData.cWindowHeight = userData.settings.window_height;
    }

    /// Key bindings
    in >> typeName;

    // Movement
    in >> t;
    in >> code;
    if (!in.fail()) userData.keyBindings.MOVE_UP = code;
    else readFailed = true;

    in >> t;
    in >> code;
    if (!in.fail()) userData.keyBindings.MOVE_DOWN = code;
    else readFailed = true;

    in >> t;
    in >> code;
    if (!in.fail()) userData.keyBindings.MOVE_LEFT = code;
    else readFailed = true;

    in >> t;
    in >> code;
    if (!in.fail()) userData.keyBindings.MOVE_RIGHT = code;
    else readFailed = true;

    // Change direction
    in >> t;
    in >> code;
    if (!in.fail()) userData.keyBindings.CHANGE_DIRECTION = code;
    else readFailed = true;

    // Change map size
    in >> t;
    in >> code;
    if (!in.fail()) userData.keyBindings.CHANGE_MAP_SIZE = code;
    else readFailed = true;

    // Inventory control
    in >> t;
    in >> code;
    if (!in.fail()) userData.keyBindings.USE_ITEM = code;
    else readFailed = true;

    in >> t;
    in >> code;
    if (!in.fail()) userData.keyBindings.DROP_ITEM = code;
    else readFailed = true;

    in >> t;
    in >> code;
    if (!in.fail()) userData.keyBindings.ITEM_1 = code;
    else readFailed = true;

    in >> t;
    in >> code;
    if (!in.fail()) userData.keyBindings.ITEM_2 = code;
    else readFailed = true;

    in >> t;
    in >> code;
    if (!in.fail()) userData.keyBindings.ITEM_3 = code;
    else readFailed = true;

    in >> t;
    in >> code;
    if (!in.fail()) userData.keyBindings.ITEM_4 = code;
    else readFailed = true;

    in >> t;
    in >> code;
    if (!in.fail()) userData.keyBindings.ITEM_5 = code;
    else readFailed = true;

    // Pause
    in >> t;
    in >> code;
    if (!in.fail()) userData.keyBindings.PAUSE = code;
    else readFailed = true;

    in.close();
}

void Game::load_menu()
{
    menu.scenes.clear();

    menu.scenes.push_back(MenuScene("main menu", "main_menu_background", 3, 3));
    {
        menu.scenes[SCENE_MAIN_MENU].elements.push_back(Element(ME_BUTTON, true, "play", true, true, SM_LINE, 0, 128));
        menu.scenes[SCENE_MAIN_MENU].elements.push_back(Element(ME_BUTTON, true, "options", true, true, SM_LINE, 0, 192));
        menu.scenes[SCENE_MAIN_MENU].elements.push_back(Element(ME_BUTTON, true, "exit", true, true, SM_LINE, 0, 256));
    }
    
    menu.scenes.push_back(MenuScene("play", "", 3, 3));
    {
        menu.scenes[SCENE_PLAY].elements.push_back(Element(ME_BUTTON, false, "new game", true, true, SM_LINE, 0, 128));
        menu.scenes[SCENE_PLAY].elements.push_back(Element(ME_BUTTON, false, "load save", true, true, SM_LINE, 0, 192));
        menu.scenes[SCENE_PLAY].elements.push_back(Element(ME_BUTTON, false, "user levels", true, true, SM_LINE, 0, 320));
    }

    menu.scenes.push_back(MenuScene("saves", "", _SAVE_FILE_NAMES.size(), _SAVE_FILE_NAMES.size()));
    {
        for (int i = 0; i < _SAVE_FILE_NAMES.size(); i++) {
            menu.scenes[SCENE_PLAY].elements[1].active = true;
            menu.scenes[SCENE_SAVES].elements.push_back(Element(ME_BUTTON, true, _SAVE_FILE_NAMES[i], true, true, SM_LINE, 0, 128 + 64 * i));
        }
    }

    menu.scenes.push_back(MenuScene("user levels", "", _USER_LEVEL_FILE_NAMES.size(), _USER_LEVEL_FILE_NAMES.size()));
    {
        for (int i = 0; i < _USER_LEVEL_FILE_NAMES.size(); i++) {
            menu.scenes[SCENE_PLAY].elements[2].active = true;
            menu.scenes[SCENE_USER_LEVELS].elements.push_back(Element(ME_BUTTON, true, _USER_LEVEL_FILE_NAMES[i], true, true, SM_LINE, 0, 128 + 64 * i));
        }
    }
    
    menu.scenes.push_back(MenuScene("options", "options_background", 2, 2));
    {
        menu.scenes[SCENE_OPTIONS].elements.push_back(Element(ME_BUTTON, true, "video", true, true, SM_LINE, 0, 128));
        menu.scenes[SCENE_OPTIONS].elements.push_back(Element(ME_BUTTON, true, "controls", true, true, SM_LINE, 0, 192));
    }

    menu.scenes.push_back(MenuScene("video", "options_background", 9, 4));
    {
        menu.scenes[SCENE_VIDEO].elements.push_back(Element(0, true, true, SM_LINE, 100, 128));
        menu.scenes[SCENE_VIDEO].elements[0].on = userData.settings.fullscreen;

        std::vector<std::string> selections;
        if (userData.screenWidth >= 1280 && userData.screenHeight >= 720) selections.push_back("1280x720");
        if (userData.screenWidth >= 1280 && userData.screenHeight >= 1024) selections.push_back("1280x1024");
        if (userData.screenWidth >= 1440 && userData.screenHeight >= 900) selections.push_back("1440x900");
        if (userData.screenWidth >= 1600 && userData.screenHeight >= 900) selections.push_back("1600x900");
        if (userData.screenWidth >= 1920 && userData.screenHeight >= 1080) selections.push_back("1920x1080");
        if (userData.screenWidth >= 2560 && userData.screenHeight >= 1440) selections.push_back("2560x1440");
        if (userData.screenWidth >= 2560 && userData.screenHeight >= 1600) selections.push_back("2560x1600");

        menu.scenes[SCENE_VIDEO].elements.push_back(Element(selections, 164, true, true, SM_LINE, 100, 192));

        menu.scenes[SCENE_VIDEO].elements.push_back(Element(0, true, true, SM_LINE, 260, 256));
        menu.scenes[SCENE_VIDEO].elements[2].on = userData.settings.native;

        std::string rs = "";
        std::ostringstream w, h;
        w << userData.settings.window_width;
        h << userData.settings.window_height;
        rs += w.str() + "x" + h.str();

        for (int i = 0; i < menu.scenes[SCENE_VIDEO].elements[1].selections.size(); i++) {
            if (menu.scenes[SCENE_VIDEO].elements[1].selections[i] == rs) {
                menu.scenes[SCENE_VIDEO].elements[1].cSelection = i;
                break;
            }
        }

        menu.scenes[SCENE_VIDEO].elements.push_back(Element(ME_BUTTON, true, "save", true, true, SM_LINE, 0, 320));

        menu.scenes[SCENE_VIDEO].elements.push_back(Element(ME_TEXT, true, "fullscreen:", true, false, SM_LINE, -200, 128));
        menu.scenes[SCENE_VIDEO].elements.push_back(Element(ME_TEXT, true, "resolution:", true, false, SM_LINE, -200, 192));
        menu.scenes[SCENE_VIDEO].elements.push_back(Element(ME_TEXT, true, "native resolution in fullscreen:", true, false, SM_LINE, -340, 256));
        menu.scenes[SCENE_VIDEO].elements.push_back(Element(ME_TEXT, true, "saved", true, true, SM_LINE, 0, userData.cWindowHeight - 100));
        menu.scenes[SCENE_VIDEO].elements[7].visible = false;
        menu.scenes[SCENE_VIDEO].elements.push_back(Element(ME_TEXT, true, "changes will apply after restart", true, true, SM_LINE, 0, userData.cWindowHeight - 60));
        menu.scenes[SCENE_VIDEO].elements[8].visible = false;
    }

    menu.scenes.push_back(MenuScene("controls", "options_background", 28, 14));
    {
        std::string kName;

        kName = keys.key_name(userData.keyBindings.MOVE_UP);
        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_BUTTON, true, kName, true, true, SM_LINE, -350 + 150, 128));
        kName = keys.key_name(userData.keyBindings.MOVE_DOWN);
        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_BUTTON, true, kName, true, true, SM_LINE, -350 + 150, 192));
        kName = keys.key_name(userData.keyBindings.MOVE_LEFT);
        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_BUTTON, true, kName, true, true, SM_LINE, -350 + 150, 256));
        kName = keys.key_name(userData.keyBindings.MOVE_RIGHT);
        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_BUTTON, true, kName, true, true, SM_LINE, -350 + 150, 320));
        kName = keys.key_name(userData.keyBindings.CHANGE_DIRECTION);
        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_BUTTON, true, kName, true, true, SM_LINE, -350 + 150, 384));
        kName = keys.key_name(userData.keyBindings.CHANGE_MAP_SIZE);
        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_BUTTON, true, kName, true, true, SM_LINE, -350 + 150, 448));
        kName = keys.key_name(userData.keyBindings.PAUSE);
        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_BUTTON, true, kName, true, true, SM_LINE, -350 + 150, 512));
        kName = keys.key_name(userData.keyBindings.USE_ITEM);
        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_BUTTON, true, kName, true, true, SM_LINE, 250 + 150, 128));
        kName = keys.key_name(userData.keyBindings.DROP_ITEM);
        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_BUTTON, true, kName, true, true, SM_LINE, 250 + 150, 192));
        kName = keys.key_name(userData.keyBindings.ITEM_1);
        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_BUTTON, true, kName, true, true, SM_LINE, 250 + 150, 256));
        kName = keys.key_name(userData.keyBindings.ITEM_2);
        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_BUTTON, true, kName, true, true, SM_LINE, 250 + 150, 320));
        kName = keys.key_name(userData.keyBindings.ITEM_3);
        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_BUTTON, true, kName, true, true, SM_LINE, 250 + 150, 384));
        kName = keys.key_name(userData.keyBindings.ITEM_4);
        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_BUTTON, true, kName, true, true, SM_LINE, 250 + 150, 448));
        kName = keys.key_name(userData.keyBindings.ITEM_5);
        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_BUTTON, true, kName, true, true, SM_LINE, 250 + 150, 512));

        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_TEXT, true, "move up:", true, false, SM_LINE, -350 - 150, 128));
        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_TEXT, true, "move down:", true, false, SM_LINE, -350 - 150, 192));
        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_TEXT, true, "move left:", true, false, SM_LINE, -350 - 150, 256));
        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_TEXT, true, "move right:", true, false, SM_LINE, -350 - 150, 320));
        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_TEXT, true, "direction:", true, false, SM_LINE, -350 - 150, 384));
        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_TEXT, true, "map size:", true, false, SM_LINE, -350 - 150, 448));
        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_TEXT, true, "pause:", true, false, SM_LINE, -350 - 150, 512));
        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_TEXT, true, "use item:", true, false, SM_LINE, 250 - 150, 128));
        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_TEXT, true, "drop item:", true, false, SM_LINE, 250 - 150, 192));
        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_TEXT, true, "item 1:", true, false, SM_LINE, 250 - 150, 256));
        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_TEXT, true, "item 2:", true, false, SM_LINE, 250 - 150, 320));
        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_TEXT, true, "item 3:", true, false, SM_LINE, 250 - 150, 384));
        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_TEXT, true, "item 4:", true, false, SM_LINE, 250 - 150, 448));
        menu.scenes[SCENE_CONTROLS].elements.push_back(Element(ME_TEXT, true, "item 5:", true, false, SM_LINE, 250 - 150, 512));
    }
}

void Game::read_save_file_names()
{
    // Save directory
    std::wstring dir = L"Content/Saves/";
    std::wstring path = dir + L"*.json*";
    
    // Find files
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(path.c_str(), &fd);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                // Convert from wstring to string and push back
                _SAVE_FILE_NAMES.push_back(wstring_to_string(fd.cFileName));

                // Remove .txt extension from names
                _SAVE_FILE_NAMES[_SAVE_FILE_NAMES.size() - 1].pop_back();
                _SAVE_FILE_NAMES[_SAVE_FILE_NAMES.size() - 1].pop_back();
                _SAVE_FILE_NAMES[_SAVE_FILE_NAMES.size() - 1].pop_back();
                _SAVE_FILE_NAMES[_SAVE_FILE_NAMES.size() - 1].pop_back();
                _SAVE_FILE_NAMES[_SAVE_FILE_NAMES.size() - 1].pop_back();
            }
        } while (::FindNextFile(hFind, &fd));
        ::FindClose(hFind);
    }
}

void Game::read_user_level_names()
{
    // Level directory
    std::wstring dir = L"Content/Userlevels";
    std::wstring path = dir + L"*.*";

    // Find files
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(path.c_str(), &fd);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                // Convert from wstring to string and push back
                _USER_LEVEL_FILE_NAMES.push_back(wstring_to_string(fd.cFileName));

                // Remove .txt extension from names
                _USER_LEVEL_FILE_NAMES[_SAVE_FILE_NAMES.size() - 1].pop_back();
                _USER_LEVEL_FILE_NAMES[_SAVE_FILE_NAMES.size() - 1].pop_back();
                _USER_LEVEL_FILE_NAMES[_SAVE_FILE_NAMES.size() - 1].pop_back();
                _USER_LEVEL_FILE_NAMES[_SAVE_FILE_NAMES.size() - 1].pop_back();
            }
        } while (::FindNextFile(hFind, &fd));
        ::FindClose(hFind);
    }
}

void Game::read_textures()
{
    std::ifstream in;
    in.open("Textures.txt");

    int typeCount;

    // Blocks
    in >> typeCount;
    for (int i = 0; i < typeCount; i++) {
        std::string fileDirectory;
        in >> fileDirectory;
        std::string spriteName;
        in >> spriteName;
        std::string fileDirectoryFull = "Content/" + fileDirectory;
        sprites.push_back(Sprite(fileDirectoryFull, spriteName));
        spriteFilePaths.push_back(fileDirectoryFull);
    }

    // Entities
    in >> typeCount;
    for (int i = 0; i < typeCount; i++) {
        std::string fileDirectory;
        in >> fileDirectory;
        std::string spriteName;
        in >> spriteName;
        std::string fileDirectoryFull = "Content/" + fileDirectory;
        sprites.push_back(Sprite(fileDirectoryFull, spriteName));
        spriteFilePaths.push_back(fileDirectoryFull);
    }

    // Player
    in >> typeCount;
    for (int i = 0; i < typeCount; i++) {
        std::string fileDirectory;
        in >> fileDirectory;
        std::string spriteName;
        in >> spriteName;
        std::string fileDirectoryFull = "Content/" + fileDirectory;
        sprites.push_back(Sprite(fileDirectoryFull, spriteName));
        spriteFilePaths.push_back(fileDirectoryFull);
    }

    // Wiring
    in >> typeCount;
    for (int i = 0; i < typeCount; i++) {
        std::string fileDirectory;
        in >> fileDirectory;
        std::string spriteName;
        in >> spriteName;
        std::string fileDirectoryFull = "Content/" + fileDirectory;
        sprites.push_back(Sprite(fileDirectoryFull, spriteName));
        spriteFilePaths.push_back(fileDirectoryFull);
    }

    // Door
    in >> typeCount;
    for (int i = 0; i < typeCount; i++) {
        std::string fileDirectory;
        in >> fileDirectory;
        std::string spriteName;
        in >> spriteName;
        std::string fileDirectoryFull = "Content/" + fileDirectory;
        sprites.push_back(Sprite(fileDirectoryFull, spriteName));
        spriteFilePaths.push_back(fileDirectoryFull);
    }

    // UI
    in >> typeCount;
    for (int i = 0; i < typeCount; i++) {
        std::string fileDirectory;
        in >> fileDirectory;
        std::string spriteName;
        in >> spriteName;
        std::string fileDirectoryFull = "Content/" + fileDirectory;
        sprites.push_back(Sprite(fileDirectoryFull, spriteName));
        spriteFilePaths.push_back(fileDirectoryFull);
    }

    // Other
    in >> typeCount;
    for (int i = 0; i < typeCount; i++) {
        std::string fileDirectory;
        in >> fileDirectory;
        std::string spriteName;
        in >> spriteName;
        std::string fileDirectoryFull = "Content/" + fileDirectory;
        sprites.push_back(Sprite(fileDirectoryFull, spriteName));
        spriteFilePaths.push_back(fileDirectoryFull);
    }

    // Menu
    in >> typeCount;
    for (int i = 0; i < typeCount; i++) {
        std::string fileDirectory;
        in >> fileDirectory;
        std::string spriteName;
        in >> spriteName;
        std::string fileDirectoryFull = "Content/" + fileDirectory;
        sprites.push_back(Sprite(fileDirectoryFull, spriteName));
        spriteFilePaths.push_back(fileDirectoryFull);
    }

    // Animations
    in >> typeCount;
    for (int i = 0; i < typeCount; i++) {
        std::string fileDirectory;
        in >> fileDirectory;
        std::string spriteName;
        in >> spriteName;
        std::string fileDirectoryFull = "Content/" + fileDirectory;
        sprites.push_back(Sprite(fileDirectoryFull, spriteName));
        spriteFilePaths.push_back(fileDirectoryFull);
    }

    // Large font
    in >> typeCount;
    for (int i = 0; i < typeCount; i++) {
        std::string fileDirectory;
        in >> fileDirectory;
        std::string spriteName;
        in >> spriteName;
        std::string fileDirectoryFull = "Content/" + fileDirectory;
        f_large.character.push_back(Sprite(fileDirectoryFull, spriteName));
        spriteFilePaths.push_back(fileDirectoryFull);
        //s_font_large.push_back(Sprite(fileDirectoryFull, spriteName));
    }

    // Small font
    in >> typeCount;
    for (int i = 0; i < typeCount; i++) {
        std::string fileDirectory;
        in >> fileDirectory;
        std::string spriteName;
        in >> spriteName;
        std::string fileDirectoryFull = "Content/" + fileDirectory;
        f_small.character.push_back(Sprite(fileDirectoryFull, spriteName));
        spriteFilePaths.push_back(fileDirectoryFull);
        //s_font_small.push_back(Sprite(fileDirectoryFull, spriteName));
    }

    return;

    // Items
    in >> typeCount;
    for (int i = 0; i < typeCount; i++) {
        std::string fileDirectory;
        in >> fileDirectory;
        std::string spriteName;
        in >> spriteName;
        std::string fileDirectoryFull = "Content/" + fileDirectory;
        sprites.push_back(Sprite(fileDirectoryFull, spriteName));
    }

    // Barbarian
    in >> typeCount;
    for (int i = 0; i < typeCount; i++) {
        std::string fileDirectory;
        in >> fileDirectory;
        std::string spriteName;
        in >> spriteName;
        std::string fileDirectoryFull = "Content/" + fileDirectory;
        sprites.push_back(Sprite(fileDirectoryFull, spriteName));
    }

    // Wizard
    in >> typeCount;
    for (int i = 0; i < typeCount; i++) {
        std::string fileDirectory;
        in >> fileDirectory;
        std::string spriteName;
        in >> spriteName;
        std::string fileDirectoryFull = "Content/" + fileDirectory;
        sprites.push_back(Sprite(fileDirectoryFull, spriteName));
    }

    // Paladin
    in >> typeCount;
    for (int i = 0; i < typeCount; i++) {
        std::string fileDirectory;
        in >> fileDirectory;
        std::string spriteName;
        in >> spriteName;
        std::string fileDirectoryFull = "Content/" + fileDirectory;
        sprites.push_back(Sprite(fileDirectoryFull, spriteName));
    }

    // Zombie
    in >> typeCount;
    for (int i = 0; i < typeCount; i++) {
        std::string fileDirectory;
        in >> fileDirectory;
        std::string spriteName;
        in >> spriteName;
        std::string fileDirectoryFull = "Content/" + fileDirectory;
        sprites.push_back(Sprite(fileDirectoryFull, spriteName));
    }

    // Lich
    in >> typeCount;
    for (int i = 0; i < typeCount; i++) {
        std::string fileDirectory;
        in >> fileDirectory;
        std::string spriteName;
        in >> spriteName;
        std::string fileDirectoryFull = "Content/" + fileDirectory;
        sprites.push_back(Sprite(fileDirectoryFull, spriteName));
    }

    // Wraith
    in >> typeCount;
    for (int i = 0; i < typeCount; i++) {
        std::string fileDirectory;
        in >> fileDirectory;
        std::string spriteName;
        in >> spriteName;
        std::string fileDirectoryFull = "Content/" + fileDirectory;
        sprites.push_back(Sprite(fileDirectoryFull, spriteName));
    }

    // Animations
    in >> typeCount;
    for (int i = 0; i < typeCount; i++) {
        std::string fileDirectory;
        in >> fileDirectory;
        std::string spriteName;
        in >> spriteName;
        std::string fileDirectoryFull = "Content/" + fileDirectory;
        sprites.push_back(Sprite(fileDirectoryFull, spriteName));
    }

    return;

    // Particles
    // General
    in >> typeCount;
    for (int i = 0; i < typeCount; i++) {
        std::string fileDirectory;
        in >> fileDirectory;
        std::string spriteName;
        in >> spriteName;
        std::string fileDirectoryFull = "Content/" + fileDirectory;
        //s_ptc_general.push_back(Sprite(fileDirectoryFull, spriteName));
    }

    // Minimap
    in >> typeCount;
    for (int i = 0; i < typeCount; i++) {
        std::string fileDirectory;
        in >> fileDirectory;
        std::string spriteName;
        in >> spriteName;
        std::string fileDirectoryFull = "Content/" + fileDirectory;
        level.minimap.cells.push_back(Sprite(fileDirectoryFull, spriteName));
    }

    in.close();
}

void Game::load_items()
{
    // Save directory
    std::wstring dir = L"Scripts/Items/";
    std::wstring path = dir + L"*.lua*";

    // Find files
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(path.c_str(), &fd);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                int index = _ITEM_SCRIPTS.size();
                std::string spath = "Scripts/Items/" + wstring_to_string(fd.cFileName);

                // Load script
                sol::state scr;
                scr.script_file(spath);

                // Add script data object
                _ITEM_SCRIPTS.push_back(ScriptLoadData());

                // Set id and file path
                _ITEM_SCRIPTS[index].id = scr["ID"];
                _ITEM_SCRIPTS[index].filePath = spath;

                // Add requested sprites
                sol::optional<int> spriteCount = scr["spritecount"];
                for (int i = 1; i <= spriteCount.value(); i++) {
                    // Get sprite path
                    std::string spritePath = scr["spritepaths"][i];

                    // Add sprite and get its position
                    int sprindex = load_sprite(spritePath);

                    // Add index to script data
                    _ITEM_SCRIPTS[index].spriteIndexes.push_back(sprindex);
                }
            }
        } while (::FindNextFile(hFind, &fd));
        ::FindClose(hFind);
    }
}

void Game::load_objects()
{
    // Save directory
    std::wstring dir = L"Scripts/Objects/";
    std::wstring path = dir + L"*.lua*";

    // Find files
    WIN32_FIND_DATA fd;
    HANDLE hFind = ::FindFirstFile(path.c_str(), &fd);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                int index = _OBJECT_SCRIPTS.size();
                std::string spath = "Scripts/Objects/" + wstring_to_string(fd.cFileName);

                // Load script
                sol::state scr;
                scr.script_file(spath);

                // Add script data object
                _OBJECT_SCRIPTS.push_back(ScriptLoadData());

                // Set id and file path
                _OBJECT_SCRIPTS[index].id        = scr["ID"];
                _OBJECT_SCRIPTS[index].filePath = spath;

                // Add requested sprites
                sol::optional<int> spriteCount = scr["spritecount"];
                for (int i = 1; i <= spriteCount.value(); i++) {
                    // Get sprite path
                    std::string spritePath = scr["spritepaths"][i];

                    // Add sprite and get its position
                    int sprindex = load_sprite(spritePath);

                    // Add index to script data
                    _OBJECT_SCRIPTS[index].spriteIndexes.push_back(sprindex);
                }
            }
        } while (::FindNextFile(hFind, &fd));
        ::FindClose(hFind);
    }
}

int Game::load_sprite(std::string fileDir)
{
    // Find duplicates
    for (int i = 0; i < sprites.size(); i++)
        if (fileDir == spriteFilePaths[i])
            return i;

    // Add new sprite
    sprites.push_back(Sprite(fileDir, ""));
    spriteFilePaths.push_back(fileDir);

    return sprites.size() - 1;
}

/*
void Game::ReadLevels()
{
    std::ifstream in;
    in.open("Content/Game/Levels/LevelNames.txt");

    in >> levelCount;

    for (int i = 0; i < levelCount; i++) {
        std::string levelFileName;
        in >> levelFileName;
        levelInfo.push_back(LevelInfo(levelFileName));
    }
}
*/