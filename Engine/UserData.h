#pragma once

#include "wtypes.h"

#include "KeyBindings.h"
#include "Settings.h"

#include <fstream>
#include <string>

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

        screenWidth = desktop.right;
        screenHeight = desktop.bottom;
    }

    void save_data()
    {
        std::ofstream out("Content/Player/config.txt");

        out << "SETTINGS" << std::endl;
        out << "fullscreen: " << settings.fullscreen << std::endl;
        out << "resolution: " << settings.window_width << " " << settings.window_height << std::endl;
        out << "native: "      << settings.native << std::endl << std::endl;

        out << "KEYBINDINGS" << std::endl;
        out << "moveup: "     << keyBindings.MOVE_UP       << std::endl;
        out << "movedown: "  << keyBindings.MOVE_DOWN  << std::endl;
        out << "moveleft: "  << keyBindings.MOVE_LEFT  << std::endl;
        out << "moveright: " << keyBindings.MOVE_RIGHT << std::endl;
        out << "changedirection: " << keyBindings.CHANGE_DIRECTION << std::endl;
        out << "changemapsize: "   << keyBindings.CHANGE_MAP_SIZE  << std::endl;
        out << "useitem: "  << keyBindings.USE_ITEM  << std::endl;
        out << "dropitem: " << keyBindings.DROP_ITEM << std::endl;
        out << "item1: " << keyBindings.ITEM_1 << std::endl;
        out << "item2: " << keyBindings.ITEM_2 << std::endl;
        out << "item3: " << keyBindings.ITEM_3 << std::endl;
        out << "item4: " << keyBindings.ITEM_4 << std::endl;
        out << "item5: " << keyBindings.ITEM_5 << std::endl;
        out << "pause: " << keyBindings.PAUSE << std::endl << std::endl;
    }
};