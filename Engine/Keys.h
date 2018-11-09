#pragma once

#include <Windows.h>
#include <string>

class Keys
{
    int keyCodes[58] =
    {
        0x08,
        0x09,
        0x0D,
        0x10,
        0x11,
        0x12,
        0x1B,
        0x20,
        0x25,
        0x26,
        0x27,
        0x28,
        0x30,
        0x31,
        0x32,
        0x33,
        0x34,
        0x35,
        0x36,
        0x37,
        0x38,
        0x39,
        0x41,
        0x42,
        0x43,
        0x44,
        0x45,
        0x46,
        0x47,
        0x48,
        0x49,
        0x4A,
        0x4B,
        0x4C,
        0x4D,
        0x4E,
        0x4F,
        0x50,
        0x51,
        0x52,
        0x53,
        0x54,
        0x55,
        0x56,
        0x57,
        0x58,
        0x59,
        0x5A,
        0x60,
        0x61,
        0x62,
        0x63,
        0x64,
        0x65,
        0x66,
        0x67,
        0x68,
        0x69
    };

    std::string keyNames[58] =
    {
        "backspace",
        "tab",
        "enter",
        "shift",
        "ctrl",
        "alt",
        "esc",
        "spacebar",
        "left arrow",
        "up arrow",
        "right arrow",
        "down arrow",
        "0",
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "a",
        "b",
        "c",
        "d",
        "e",
        "f",
        "g",
        "h",
        "i",
        "j",
        "k",
        "l",
        "m",
        "n",
        "o",
        "p",
        "q",
        "r",
        "s",
        "t",
        "u",
        "v",
        "w",
        "x",
        "y",
        "z",
        "num 0",
        "num 1",
        "num 2",
        "num 3",
        "num 4",
        "num 5",
        "num 6",
        "num 7",
        "num 8",
        "num 9"
    };
    
    char keyCharacters[58] =
    {
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        ' ',
        0,
        0,
        0,
        0,
        '0',
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
        'a',
        'b',
        'c',
        'd',
        'e',
        'f',
        'g',
        'h',
        'i',
        'j',
        'k',
        'l',
        'm',
        'n',
        'o',
        'p',
        'q',
        'r',
        's',
        't',
        'u',
        'v',
        'w',
        'x',
        'y',
        'z',
        '0',
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9'
    };

    int state[58] = { 0 }; // 0 - not pressed, 1 - was pressed this cycle, 2 - is pressed for more than 1 cycle

public:

    Keys() {};
    
    void update_key_states()
    {
        for (int i = 0; i < 58; i++) {
            bool p = GetKeyState(keyCodes[i]) & 0x8000;

            if        (p && state[i] == 0) state[i] = 1;
            else if (p && state[i] == 1) state[i] = 2;
            else if (!p)                 state[i] = 0;
        }
    }

    bool key_state(int code)
    {
        for (int i = 0; i < 58; i++) {
            if (keyCodes[i] == code) {
                if (state[i] == 1) 
                    return true;
                else
                    return false;
            }
        }
        return false;
    }

    bool key_state_true(int code)
    {
        for (int i = 0; i < 58; i++) {
            if (keyCodes[i] == code) {
                if (state[i] == 1 || state[i] == 2)
                    return true;
                else
                    return false;
            }
        }
        return false;
    }

    bool pressed_key_code(int &code)
    {
        for (int i = 0; i < 58; i++) {
            if (state[i] == 1) {
                code = keyCodes[i];
                return true;
            }
        }
        return false;
    }

    std::string key_name(int code)
    {
        for (int i = 0; i < 58; i++) {
            if (keyCodes[i] == code) {
                return keyNames[i];
            }
        }
        return "unknown";
    }

    char key_character(int code)
    {
        for (int i = 0; i < 58; i++) {
            if (keyCodes[i] == code) {
                return keyCharacters[i];
            }
        }
        return 0;
    }

    int key_code(std::string name)
    {
        for (int i = 0; i < 58; i++) {
            if (keyNames[i] == name) {
                return keyCodes[i];
            }
        }
        return -1;
    }
};

class SmartText
{
    std::string text;

    std::string whitelist;
public:
    bool locked = false;

    SmartText()
    {
        this->whitelist = " 0123456789abcdefghijklmnopqrstuvwxyz";
    }
    SmartText(bool allowSpaces, bool allowNumbers, bool allowLetters)
    {
        whitelist = "";
        if (allowSpaces)
            whitelist += " ";
        if (allowNumbers)
            whitelist += "0123456789";
        if (allowLetters)
            whitelist += "abcdefghijklmnopqrstuvwxyz";
    }
    SmartText(std::string whitelist)
    {
        this->whitelist = whitelist;
    }

    void update_text(Keys &k)
    {
        // Don't update if text is locked
        if (locked) return;

        // Lock text if enter is pressed
        if (k.key_state(0x20)) {
            locked = true;
            return;
        }

        // If backspace is pressed, remove 1 character from string
        if (k.key_state(0x08)) {
            text.pop_back();
            return;
        }

        // Get pressed character
        char c;

        //for (int i = 0; i < whitelist)
    }
};