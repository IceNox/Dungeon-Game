#pragma once

#include "ChiliWin.h"
#include <string>

// Constants
const int MAX_INT = 2147483647;
const int KEY_COUNT = 121;
const int CYCLES_HELD = 20;
const int TEXT_RESERVE = 30;

namespace kb
{
    namespace
    {
        enum KeyCodesEnum
        {
            KC_LBUTTON = 0x01,
            KC_RBUTTON = 0x02,
            KC_MBUTTON = 0x04,
            KC_XBUTTON1 = 0x05,
            KC_XBUTTON2 = 0x06,
            KC_BACKSPACE = 0x08,
            KC_TAB = 0x09,
            KC_CLEAR = 0x0C,
            KC_ENTER = 0x0D,
            KC_RIGHT_ALT = 0x11,
            KC_LEFT_ALT = 0x12,
            KC_PAUSE = 0x13,
            KC_CAPS_LOCK = 0x14,
            KC_ESC = 0x1B,
            KC_SPACEBAR = 0x20,
            KC_PAGE_UP = 0x21,
            KC_PAGE_DOWN = 0x22,
            KC_END = 0x23,
            KC_HOME = 0x24,
            KC_LEFT_ARROW = 0x25,
            KC_UP_AROW = 0x26,
            KC_RIGHT_ARROW = 0x27,
            KC_DOWN_ARROW = 0x28,
            KC_PRINT_SCREEN = 0x2C,
            KC_INSERT = 0x2D,
            KC_DELETE = 0x2E,
            KC_0 = 0x30,
            KC_1 = 0x31,
            KC_2 = 0x32,
            KC_3 = 0x33,
            KC_4 = 0x34,
            KC_5 = 0x35,
            KC_6 = 0x36,
            KC_7 = 0x37,
            KC_8 = 0x38,
            KC_9 = 0x39,
            KC_A = 0x41,
            KC_B = 0x42,
            KC_C = 0x43,
            KC_D = 0x44,
            KC_E = 0x45,
            KC_F = 0x46,
            KC_G = 0x47,
            KC_H = 0x48,
            KC_I = 0x49,
            KC_J = 0x4A,
            KC_K = 0x4B,
            KC_L = 0x4C,
            KC_M = 0x4D,
            KC_N = 0x4E,
            KC_O = 0x4F,
            KC_P = 0x50,
            KC_Q = 0x51,
            KC_R = 0x52,
            KC_S = 0x53,
            KC_T = 0x54,
            KC_U = 0x55,
            KC_V = 0x56,
            KC_W = 0x57,
            KC_X = 0x58,
            KC_Y = 0x59,
            KC_Z = 0x5A,
            KC_LEFT_WIN = 0x5B,
            KC_RIGHT_WIN = 0x5C,
            KC_MENU = 0x5D,
            KC_NUM_0 = 0x60,
            KC_NUM_1 = 0x61,
            KC_NUM_2 = 0x62,
            KC_NUM_3 = 0x63,
            KC_NUM_4 = 0x64,
            KC_NUM_5 = 0x65,
            KC_NUM_6 = 0x66,
            KC_NUM_7 = 0x67,
            KC_NUM_8 = 0x68,
            KC_NUM_9 = 0x69,
            KC_MULTIPLY = 0x6A,
            KC_ADD = 0x6B,
            KC_SUBTRACT = 0x6D,
            KC_DECIMAL = 0x6E,
            KC_DIVIDE = 0x6F,
            KC_FUN_1 = 0x70,
            KC_FUN_2 = 0x71,
            KC_FUN_3 = 0x72,
            KC_FUN_4 = 0x73,
            KC_FUN_5 = 0x74,
            KC_FUN_6 = 0x75,
            KC_FUN_7 = 0x76,
            KC_FUN_8 = 0x77,
            KC_FUN_9 = 0x78,
            KC_FUN_10 = 0x79,
            KC_FUN_11 = 0x7A,
            KC_FUN_12 = 0x7B,
            KC_FUN_13 = 0x7C,
            KC_FUN_14 = 0x7D,
            KC_FUN_15 = 0x7E,
            KC_FUN_16 = 0x7F,
            KC_FUN_17 = 0x80,
            KC_FUN_18 = 0x81,
            KC_FUN_19 = 0x82,
            KC_FUN_20 = 0x83,
            KC_FUN_21 = 0x84,
            KC_FUN_22 = 0x85,
            KC_FUN_23 = 0x86,
            KC_FUN_24 = 0x87,
            KC_NUM_LOCK = 0x90,
            KC_SCROLL_LOCK = 0x91,
            KC_LEFT_SHIFT = 0xA0,
            KC_RIGHT_SHIFT = 0xA1,
            KC_LEFT_CTRL = 0xA2,
            KC_RIGHT_CTRL = 0xA3,
            KC_SEMICOLON = 0xBA,
            KC_EQUALS = 0xBB,
            KC_COMMA = 0xBC,
            KC_HYPHEN = 0xBD,
            KC_DOT = 0xBE,
            KC_FORWARD_SLASH = 0xBF,
            KC_OPEN_BRACKET = 0xDB,
            KC_BACKWARD_SLASH = 0xDC,
            KC_CLOSE_BRACKET = 0xDD,
            KC_NUMBER_SIGN = 0xDE,
            KC_APOSTROPHE = 0xDF
        };

        const int keyCodes[KEY_COUNT] =
        {
            0x01,
            0x02,
            0x04,
            0x05,
            0x06,
            0x08,
            0x09,
            0x0C,
            0x0D,
            0x11,
            0x12,
            0x13,
            0x14,
            0x1B,
            0x20,
            0x21,
            0x22,
            0x23,
            0x24,
            0x25,
            0x26,
            0x27,
            0x28,
            0x2C,
            0x2D,
            0x2E,
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
            0x5B,
            0x5C,
            0x5D,
            0x60,
            0x61,
            0x62,
            0x63,
            0x64,
            0x65,
            0x66,
            0x67,
            0x68,
            0x69,
            0x6A,
            0x6B,
            0x6D,
            0x6E,
            0x6F,
            0x70,
            0x71,
            0x72,
            0x73,
            0x74,
            0x75,
            0x76,
            0x77,
            0x78,
            0x79,
            0x7A,
            0x7B,
            0x7C,
            0x7D,
            0x7E,
            0x7F,
            0x80,
            0x81,
            0x82,
            0x83,
            0x84,
            0x85,
            0x86,
            0x87,
            0x90,
            0x91,
            0xA0,
            0xA1,
            0xA2,
            0xA3,
            0xBA,
            0xBB,
            0xBC,
            0xBD,
            0xBE,
            0xBF,
            0xDB,
            0xDC,
            0xDD,
            0xDE,
            0xDF
        };

        const std::string keyNames[KEY_COUNT] =
        {
            "mouse 1",
            "mouse 2",
            "mouse 3",
            "mouse 4",
            "mouse 5",
            "backspace",
            "tab",
            "clear",
            "enter",
            "right alt",
            "left alt",
            "pause",
            "caps lock",
            "esc",
            "spacebar",
            "page up",
            "page down",
            "end",
            "home",
            "left arrow",
            "up arrow",
            "right arrow",
            "down arrow",
            "print screen",
            "insert",
            "delete",
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
            "left win key",
            "right win key",
            "menu",
            "num 0",
            "num 1",
            "num 2",
            "num 3",
            "num 4",
            "num 5",
            "num 6",
            "num 7",
            "num 8",
            "num 9",
            "multiply",
            "add",
            "subtract",
            "decimal",
            "divide",
            "f1",
            "f2",
            "f3",
            "f4",
            "f5",
            "f6",
            "f7",
            "f8",
            "f9",
            "f10",
            "f11",
            "f12",
            "f13",
            "f14",
            "f15",
            "f16",
            "f17",
            "f18",
            "f19",
            "f20",
            "f21",
            "f22",
            "f23",
            "f24",
            "num lock",
            "scroll lock",
            "left shift",
            "right shift",
            "left ctrl",
            "right ctrl",
            ";",
            "=",
            ",",
            "-",
            ".",
            "/",
            "[",
            "\\",
            "]",
            "#",
            "`"
        };

        const char keyCharacters[KEY_COUNT] =
        {
            0,
            0,
            0,
            0,
            0,
            0,
            0,
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
            0,
            0,
            0,
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
            '*',
            '+',
            '-',
            '.',
            '/',
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            ';',
            '=',
            ',',
            '-',
            '.',
            '/',
            '[',
            '\\',
            ']',
            '#',
            '`'
        };
    }

    // Key class stores key states and functions for accessing them 
    class Keys
    {
    private:
        // 0 - not pressed, 1+ cycles that it was held down for
        int state[KEY_COUNT];

    public:
        Keys() {};

        void update_key_states()
        {
            for (int i = 0; i < KEY_COUNT; i++) {
                bool p = GetKeyState(keyCodes[i]) & 0x8000;

                if (p) state[i]++;
                else state[i] = 0;
            }
        }

        int key_state(int code)
        {
            for (int i = 0; i < KEY_COUNT; i++) {
                if (keyCodes[i] == code) {
                    return state[i];
                }
            }

            return NULL;
        }

        int index_state(int index)
        {
            return state[index];
        }

        bool pressed_key_code(int &code)
        {
            for (int i = 0; i < KEY_COUNT; i++) {
                if (state[i] == 1) {
                    code = keyCodes[i];
                    return true;
                }
            }

            return false;
        }

        bool key_clicked(int code)
        {
            return key_state(code) == 1;
        }

        bool key_held(int code)
        {
            return key_state(code) >= CYCLES_HELD;
        }

        bool index_clicked(int index)
        {
            return index_state(index) == 1;
        }

        bool index_held(int index)
        {
            return index_state(index) >= CYCLES_HELD;
        }
    };

    // Helper functions for converting between indices, codes, names and characters
    int index_to_code(int index);
    char index_to_char(int index);
    std::string index_to_name(int index);
    int code_to_index(int code);
    char code_to_char(int code);
    std::string code_to_name(int code);
    int name_to_code(std::string name);

    // Class for entering text in real-time
    class SmartText
    {
    private:
        std::string _text;
        std::string _prefix;
        std::string _whitelist;
        int _cursor;
        bool _locked;

    public:
        static const std::string spaces;
        static const std::string numbers;
        static const std::string letters;
        static const std::string special;

        SmartText();
        SmartText(std::string prefix);
        SmartText(std::string text, std::string prefix);
        std::string get_text();
        std::string get_prefix();
        std::string get_content();
        void setWhitelist();
        void setWhitelist(bool allowSpaces, bool allowNumbers, bool allowLetters, bool allowSpecial);
        void setWhitelist(std::string whitelist);
        void update_text(Keys &k);
        void clear_text();
        void set_text(std::string text);
        void clear_prefix();
        void set_prefix(std::string prefix);
        void toggle_lock();
    };
}
