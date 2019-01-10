#pragma once

#include "ChiliWin.h"
#include <string>

// Constants
const int MAX_INT = 2147483647;
const int KEY_COUNT = 58;
const int CYCLES_HELD = 20;
const int TEXT_RESERVE = 30;

namespace kb
{
    namespace
    {
        enum KeyCodesEnum
        {
            KC_BACKSPACE = 0x08,
            KC_TAB = 0x09,
            KC_ENTER = 0x0D,
            KC_SHIFT = 0x10,
            KC_CTRL = 0x11,
            KC_ALT = 0x12,
            KC_ESC = 0x1B,
            KC_SPACEBAR = 0x20,
            KC_LEFT_ARROW = 0x25,
            KC_UP_AROW = 0x26,
            KC_RIGHT_ARROW = 0x27,
            KC_DOWN_ARROW = 0x28,
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
            KC_NUM_0 = 0x60,
            KC_NUM_1 = 0x61,
            KC_NUM_2 = 0x62,
            KC_NUM_3 = 0x63,
            KC_NUM_4 = 0x64,
            KC_NUM_5 = 0x65,
            KC_NUM_6 = 0x66,
            KC_NUM_7 = 0x67,
            KC_NUM_8 = 0x68,
            KC_NUM_9 = 0x69
        };

        const int keyCodes[KEY_COUNT] =
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

        const std::string keyNames[KEY_COUNT] =
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

        const char keyCharacters[KEY_COUNT] =
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
                if (state[i]) {
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
