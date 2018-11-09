#pragma once

#include <string>

class KeyBindings
{
public:
    /// Default virtual-key codes

    // Menu
    const int UP    = 0x26;
    const int DOWN  = 0x28;
    const int LEFT  = 0x25;
    const int RIGHT = 0x27;

    const int ENTER  = 0x0D;
    const int ESCAPE = 0x1B;

    // Game
    int MOVE_UP       = 0x26;
    int MOVE_DOWN  = 0x28;
    int MOVE_LEFT  = 0x25;
    int MOVE_RIGHT = 0x27;

    int CHANGE_DIRECTION = 0x10;
    int CHANGE_MAP_SIZE = 0x4D;

    int USE_ITEM  = 0x20;
    int DROP_ITEM = 0x12;
    int ITEM_1 = 0x31;
    int ITEM_2 = 0x32;
    int ITEM_3 = 0x33;
    int ITEM_4 = 0x34;
    int ITEM_5 = 0x35;

    int PAUSE = 0x1B;

    KeyBindings() {};
    /*
    std::string get_key_name(int code)
    {
        switch (code) {
            case(0x08):
                return "backspace";
            case(0x09):
                return "tab";
            case(0x0D):
                return "enter";
            case(0x10):
                return "shift";
            case(0x11):
                return "ctrl";
            case(0x12):
                return "alt";
            case(0x1B):
                return "esc";
            case(0x20):
                return "spacebar";
            case(0x25):
                return "left arrow";
            case(0x26):
                return "up arrow";
            case(0x27):
                return "right arrow";
            case(0x28):
                return "down arrow";
            case(0x30):
                return "0";
            case(0x31):
                return "1";
            case(0x32):
                return "2";
            case(0x33):
                return "3";
            case(0x34):
                return "4";
            case(0x35):
                return "5";
            case(0x36):
                return "6";
            case(0x37):
                return "7";
            case(0x38):
                return "8";
            case(0x39):
                return "9";
            case(0x41):
                return "a";
            case(0x42):
                return "b";
            case(0x43):
                return "c";
            case(0x44):
                return "d";
            case(0x45):
                return "e";
            case(0x46):
                return "f";
            case(0x47):
                return "g";
            case(0x48):
                return "h";
            case(0x49):
                return "i";
            case(0x4A):
                return "j";
            case(0x4B):
                return "k";
            case(0x4C):
                return "l";
            case(0x4D):
                return "m";
            case(0x4E):
                return "n";
            case(0x4F):
                return "o";
            case(0x50):
                return "p";
            case(0x51):
                return "q";
            case(0x52):
                return "r";
            case(0x53):
                return "s";
            case(0x54):
                return "t";
            case(0x55):
                return "u";
            case(0x56):
                return "v";
            case(0x57):
                return "w";
            case(0x58):
                return "x";
            case(0x59):
                return "y";
            case(0x5A):
                return "z";
            case(0x60):
                return "num 0";
            case(0x61):
                return "num 1";
            case(0x62):
                return "num 2";
            case(0x63):
                return "num 3";
            case(0x64):
                return "num 4";
            case(0x65):
                return "num 5";
            case(0x66):
                return "num 6";
            case(0x67):
                return "num 7";
            case(0x68):
                return "num 8"; 
            case(0x69):
                return "num 9";
        }
    }
    */
};