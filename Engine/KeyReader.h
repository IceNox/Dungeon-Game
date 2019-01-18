#pragma once

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#include "Key.h"

#include <fstream>
#include <vector>
#include <algorithm>

// Constants
const int MAX_INT = 2147483647;
const int STATE_SHIFT = 0x8000;
const int CYCLES_HELD = 20;
const int KEY_TOTAL = 256;

const std::string LAYOUT = "Content/Misc/Layouts/UK-Layout.bin2";
const std::bitset<KEY_TOTAL> IS_SPECIAL(
    "0000000000000000"
    "0000000000000000"
    "0000000000000000"
    "0000000000000000"
    "0000000000000000"
    "0000000000111111"
    "0000000000000011"
    "0000000011111111"
    "1111111111111111"
    "1110111111111111"
    "0011100000000000"
    "0000000000000000"
    "0000000000000000"
    "0111000111111111"
    "0000100000011111"
    "0011001101110110"
);

class KeyReader
{
private:
    int _state[KEY_TOTAL] = {};
    char _layout[KEY_TOTAL][2][2][2] = {};

    // Calculates the amount of ticks a mofier has been pressed down
    int _modifier_ticks(int left, int right) {
        if (_state[left] == 0) {
            return _state[right];
        }
        else if (_state[right] == 0) {
            return _state[left];
        }
        else {
            return min(_state[left], _state[right]);
        }
    }

    // Amount of cycles the last modifier has been clicked
    int _last_modifier() {
        int shift = _modifier_ticks(VK_LSHIFT, VK_RSHIFT);
        int control = _modifier_ticks(VK_LCONTROL, VK_RCONTROL);
        int menu = _modifier_ticks(VK_LMENU, VK_RMENU);
        int last = MAX_INT;

        if (shift != 0 && shift < last) {
            last = shift;
        }

        if (control != 0 && control < last) {
            last = control;
        }

        if (menu != 0 && menu < last) {
            last = menu;
        }

        return last;
    }

    // Key code of last pressed button
    int _last_button() {
        int last = _last_modifier();
        int btn = 0;

        for (int i = 0; i < VK_SHIFT; i++) {
            if (_state[i] != 0 && _state[i] <= last) {
                last = last;
                btn = i;
            }
        }

        for (int i = 1 + VK_MENU; i < VK_LWIN; i++) {
            if (_state[i] != 0 && _state[i] <= last) {
                last = last;
                btn = i;
            }
        }

        for (int i = 1 + VK_RWIN; i < VK_LSHIFT; i++) {
            if (_state[i] != 0 && _state[i] <= last) {
                last = last;
                btn = i;
            }
        }

        for (int i = 1 + VK_RMENU; i < KEY_TOTAL; i++) {
            if (_state[i] != 0 && _state[i] <= last) {
                last = last;
                btn = i;
            }
        }

        return btn;
    }

public:
    KeyReader()
        :
        _state()
    {
        std::string line;
        std::ifstream layout(LAYOUT);

        if (layout.is_open()) {
            while (getline(layout, line)) {
                bool shift = (bool) std::stoi(line.substr(0 , 1), nullptr, 2);
                bool ctrl  = (bool) std::stoi(line.substr(1 , 1), nullptr, 2);
                bool alt   = (bool) std::stoi(line.substr(2 , 1), nullptr, 2);
                int  vk    = (int)  std::stoi(line.substr(3 , 8), nullptr, 2);
                char c     = (char) std::stoi(line.substr(11, 8), nullptr, 2);
                _layout[vk][shift][ctrl][alt] = c;
            }
        }

        layout.close();
    }

    void update_key_states()
    {
        for (int i = 0; i < KEY_TOTAL; i++) {
            if (GetKeyState(i) & STATE_SHIFT) {
                _state[i]++;
            }
            else {
                _state[i] = 0;
            }
        }
    }

    Key get_key()
    {
        int key_count = 0;
        int last = MAX_INT;
        int key = 0;

        // Intentional decrementation since single modifier key
        // VK codes are higher than shared modifier VK codes
        for (int i = KEY_TOTAL - 1; i > 0; i--) {
            int state_ticks = _state[i];
            if (state_ticks) {
                key_count++;

                if (state_ticks < last) {
                    last = state_ticks;
                    key = i;
                }
            }
        }

        if (key_count > 2) {
            key = _last_button();

            int  key_code = key * (int)(_state[key] == 1 || _state[key] > CYCLES_HELD);
            bool key_held = _state[key] > CYCLES_HELD;
            bool shift    = (bool)_modifier_ticks(VK_LSHIFT, VK_RSHIFT);
            bool ctrl     = (bool)_modifier_ticks(VK_LCONTROL, VK_RCONTROL);
            bool alt      = (bool)_modifier_ticks(VK_LMENU, VK_RMENU);

            return Key(
                shift, ctrl, alt, key_code, key_held,
                _layout[key_code][shift][ctrl][alt],
                _layout[key_code][false][false][false]
            );
        }
        else {
            int  key_code = key * (int)(_state[key] == 1 || _state[key] > CYCLES_HELD);
            bool key_held = _state[key] > CYCLES_HELD;
            char key_char = _layout[key_code][false][false][false];

            return Key(false, false, false, key_code, key_held, key_char, key_char);
        }
    }
};
