#pragma once

#include "wtypes.h"

#include "KeyBindings.h"
#include "Settings.h"

#include "json.hpp"

#include <fstream>
#include <string>

using json = nlohmann::json;

class UserData
{
public:
    std::string nickname = "player";

    int currentLevel = 1;

    KeyBindings keyBindings;
    Settings settings;

    int screenWidth;
    int screenHeight;

    int cWindowWidth;
    int cWindowHeight;

    UserData()
    {
        RECT desktop;
        const HWND hDesktop = GetDesktopWindow();
        GetWindowRect(hDesktop, &desktop);

        screenWidth  = desktop.right;
        screenHeight = desktop.bottom;
    }

    void save_data()
    {
        std::ofstream out("Content/User/config.json");
        json json_output;

        json_output["settings"]["fullscreen"] = settings.fullscreen;
        json_output["settings"]["native"]     = settings.native;
        json_output["settings"]["mode4k"]     = settings.mode4k;

        json_output["settings"]["resolution"]["width"]  = settings.window_width;
        json_output["settings"]["resolution"]["height"] = settings.window_height;

        json_output["keybindings"]["moveup"]    = keyBindings.MOVE_UP;
        json_output["keybindings"]["movedown"]  = keyBindings.MOVE_DOWN;
        json_output["keybindings"]["moveleft"]  = keyBindings.MOVE_LEFT;
        json_output["keybindings"]["moveright"] = keyBindings.MOVE_RIGHT;
        json_output["keybindings"]["changedir"] = keyBindings.CHANGE_DIRECTION;
        json_output["keybindings"]["changemap"] = keyBindings.CHANGE_MAP_SIZE;
        json_output["keybindings"]["useitem"]   = keyBindings.USE_ITEM;
        json_output["keybindings"]["dropitem"]  = keyBindings.DROP_ITEM;
        json_output["keybindings"]["item1"]     = keyBindings.ITEM_1;
        json_output["keybindings"]["item2"]     = keyBindings.ITEM_2;
        json_output["keybindings"]["item3"]     = keyBindings.ITEM_3;
        json_output["keybindings"]["item4"]     = keyBindings.ITEM_4;
        json_output["keybindings"]["item5"]     = keyBindings.ITEM_5;
        json_output["keybindings"]["pause"]     = keyBindings.PAUSE;

        out << json_output;
    }
};
