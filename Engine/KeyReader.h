#pragma once

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#include "Key.h"

#include <algorithm>

// Constants
const int MAX_INT = 2147483647;
const int STATE_SHIFT = 0x80;
const int CYCLES_HELD = 20;

class KeyReader
{
private:
    int _keyDurationState[KEY_TOTAL];
    BYTE _lpKeyState[KEY_TOTAL];
    DWORD _threadID;
    HKL _current_layout;

    // Calculates the amount of ticks a mofier has been pressed down
    int _modifier_ticks(int left, int right) {
        if (_keyDurationState[left] == 0) {
            return _keyDurationState[right];
        }
        else if (_keyDurationState[right] == 0) {
            return _keyDurationState[left];
        }
        else {
            return min(_keyDurationState[left], _keyDurationState[right]);
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
    UINT _last_button() {
        int last = _last_modifier();
        UINT uVirtKey = 0;

        for (UINT i = 0; i < VK_SHIFT; i++) {
            if (_keyDurationState[i] != 0 && _keyDurationState[i] <= last) {
                last = _keyDurationState[i];
                uVirtKey = i;
            }
        }

        for (UINT i = 1 + VK_MENU; i < VK_LWIN; i++) {
            if (_keyDurationState[i] != 0 && _keyDurationState[i] <= last) {
                last = _keyDurationState[i];
                uVirtKey = i;
            }
        }

        for (UINT i = 1 + VK_RWIN; i < VK_LSHIFT; i++) {
            if (_keyDurationState[i] != 0 && _keyDurationState[i] <= last) {
                last = _keyDurationState[i];
                uVirtKey = i;
            }
        }

        for (UINT i = 1 + VK_RMENU; i < KEY_TOTAL; i++) {
            if (_keyDurationState[i] != 0 && _keyDurationState[i] <= last) {
                last = _keyDurationState[i];
                uVirtKey = i;
            }
        }

        return uVirtKey;
    }

public:
    KeyReader(HWND hWnd)
        :
        _keyDurationState(),
        _lpKeyState()
    {
        _threadID = GetWindowThreadProcessId(hWnd, NULL);
    }

    void update_key_states()
    {
        _current_layout = GetKeyboardLayout(_threadID);

        BOOL success = GetKeyboardState(_lpKeyState);
        if (success) {
            for (int i = 0; i < KEY_TOTAL; i++) {
                if (_lpKeyState[i] & STATE_SHIFT) {
                    _keyDurationState[i]++;
                }
                else {
                    _keyDurationState[i] = 0;
                }
            }
        }
    }

    Key get_key()
    {
        UINT key = _last_button();

        bool key_held = _keyDurationState[key] > CYCLES_HELD;
        UINT uVirtKey = key * (int)(_keyDurationState[key] == 1 || key_held);
        bool shift    = (bool)_modifier_ticks(VK_LSHIFT, VK_RSHIFT);
        bool ctrl     = (bool)_modifier_ticks(VK_LCONTROL, VK_RCONTROL);
        bool alt      = (bool)_modifier_ticks(VK_LMENU, VK_RMENU);
        UINT uScanCode = MapVirtualKeyExA(uVirtKey, MAPVK_VK_TO_VSC, _current_layout);
        BYTE *lpKeyState = _lpKeyState;
        WORD lpChar = 0;
        UINT uFlags = 0;

        int success = ToAsciiEx(uVirtKey, uScanCode, lpKeyState, &lpChar, uFlags, _current_layout);
        WORD parentChar = MapVirtualKeyExA(uVirtKey, MAPVK_VK_TO_CHAR, _current_layout);
        return Key(shift, ctrl, alt, uVirtKey, key_held, lpChar, parentChar);
    }
};
