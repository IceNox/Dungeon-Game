#pragma once

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#include "Key.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

// Constants
const int MAX_INT = 2147483647;
const int STATE_SHIFT = 0x8000;
const int CYCLES_HELD = 20;

const std::string LAYOUT = "Content/Misc/Layouts/UK-Layout.bin";

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
            if (_state[i] != 0 && _state[i] < last) {
                last = _state[i];
                btn = i;
            }
        }

        for (int i = 1 + VK_MENU; i < VK_LWIN; i++) {
            if (_state[i] != 0 && _state[i] < last) {
                last = _state[i];
                btn = i;
            }
        }

        for (int i = 1 + VK_RWIN; i < VK_LSHIFT; i++) {
            if (_state[i] != 0 && _state[i] < last) {
                last = _state[i];
                btn = i;
            }
        }

        for (int i = 1 + VK_RMENU; i < KEY_TOTAL; i++) {
            if (_state[i] != 0 && _state[i] < last) {
                last = _state[i];
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
        std::ifstream layout(LAYOUT, ios::binary|ios::ate);
        std::ifstream::pos_type pos = layout.tellg();
        std::vector<char> result(pos);

        layout.seekg(0, ios::beg);
        layout.read(&result[0], pos);

        for (unsigned index = 0; index < result.size();) {
            uint8_t shift = result[index++];
            uint8_t ctrl  = result[index++];
            uint8_t alt   = result[index++];
            uint8_t vk    = result[index++];

            _layout[vk][shift][ctrl][alt] = result[index++];
        }
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
