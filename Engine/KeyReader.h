#pragma once

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#include "Key.h"

#include <algorithm>
#include <unordered_map>

const int MAX_INT2 = 2147483647;
const int STATE_SHIFT = 0x8000;
const int KEY_TOTAL = 256;
const int CYCLES_HELD = 20;

const std::unordered_map<int, std::string> MODIFIER_KEYS = {
    { VK_LSHIFT   , "left shift"    },
    { VK_RSHIFT   , "right shift"   },
    { VK_SHIFT    , "shift"         },
    { VK_LCONTROL , "left control"  },
    { VK_RCONTROL , "right control" },
    { VK_CONTROL  , "control"       },
    { VK_LMENU    , "left alt"      },
    { VK_RMENU    , "right alt"     },
    { VK_MENU     , "alt"           },
    { VK_LWIN     , "left win"      },
    { VK_RWIN     , "right win"     }
};

const std::unordered_map<int, std::string> SPECIAL_KEYS = {
    { VK_LBUTTON  , "mouse 1"       },
    { VK_RBUTTON  , "mouse 2"       },
    { VK_MBUTTON  , "mouse 3"       },
    { VK_XBUTTON1 , "mouse 4"       },
    { VK_XBUTTON2 , "mouse 5"       },
    { VK_BACK     , "backspace"     },
    { VK_TAB      , "tab"           },
    { VK_CLEAR    , "clear"         },
    { VK_RETURN   , "enter"         },
    { VK_PAUSE    , "pause"         },
    { VK_CAPITAL  , "caps lock"     },
    { VK_ESCAPE   , "esc"           },
    { VK_SPACE    , "spacebar"      },
    { VK_PRIOR    , "page up"       },
    { VK_NEXT     , "page down"     },
    { VK_END      , "end"           },
    { VK_HOME     , "home"          },
    { VK_LEFT     , "left arrow"    },
    { VK_UP       , "up arrow"      },
    { VK_RIGHT    , "right arrow"   },
    { VK_DOWN     , "down arrow"    },
    { VK_SNAPSHOT , "print screen"  },
    { VK_INSERT   , "insert"        },
    { VK_DELETE   , "delete"        },
    { VK_APPS     , "menu"          },
    { VK_NUMPAD0  , "num 0"         },
    { VK_NUMPAD1  , "num 1"         },
    { VK_NUMPAD2  , "num 2"         },
    { VK_NUMPAD3  , "num 3"         },
    { VK_NUMPAD4  , "num 4"         },
    { VK_NUMPAD5  , "num 5"         },
    { VK_NUMPAD6  , "num 6"         },
    { VK_NUMPAD7  , "num 7"         },
    { VK_NUMPAD8  , "num 8"         },
    { VK_NUMPAD9  , "num 9"         },
    { VK_MULTIPLY , "multiply"      },
    { VK_ADD      , "add"           },
    { VK_SUBTRACT , "subtract"      },
    { VK_DECIMAL  , "decimal"       },
    { VK_DIVIDE   , "divide"        },
    { VK_F1       , "f1"            },
    { VK_F2       , "f2"            },
    { VK_F3       , "f3"            },
    { VK_F4       , "f4"            },
    { VK_F5       , "f5"            },
    { VK_F6       , "f6"            },
    { VK_F7       , "f7"            },
    { VK_F8       , "f8"            },
    { VK_F9       , "f9"            },
    { VK_F10      , "f10"           },
    { VK_F11      , "f11"           },
    { VK_F12      , "f12"           },
    { VK_F13      , "f13"           },
    { VK_F14      , "f14"           },
    { VK_F15      , "f15"           },
    { VK_F16      , "f16"           },
    { VK_F17      , "f17"           },
    { VK_F18      , "f18"           },
    { VK_F19      , "f19"           },
    { VK_F20      , "f20"           },
    { VK_F21      , "f21"           },
    { VK_F22      , "f22"           },
    { VK_F23      , "f23"           },
    { VK_F24      , "f24"           },
    { VK_NUMLOCK  , "num lock"      },
    { VK_SCROLL   , "scroll lock"   }
};

class KeyReader
{
private:
    int _state[KEY_TOTAL];

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
        int win = _modifier_ticks(VK_LWIN, VK_RWIN);
        int last = MAX_INT2;

        if (shift != 0 && shift < last) {
            last = shift;
        }

        if (control != 0 && control < last) {
            last = control;
        }

        if (menu != 0 && menu < last) {
            last = menu;
        }

        if (win != 0 && win < last) {
            last = win;
        }

        return last;
    }

    // Key code of last pressed button
    int _last_button() {
        int last = _last_modifier();
        int btn = 0;

        for (int i = 0; i < VK_SHIFT; i++) {
            if (_state[i] != 0 && _state[i] < last) {
                last = last;
                btn = i;
            }
        }

        for (int i = 1 + VK_MENU; i < VK_LWIN; i++) {
            if (_state[i] != 0 && _state[i] < last) {
                last = last;
                btn = i;
            }
        }

        for (int i = 1 + VK_RWIN; i < VK_LSHIFT; i++) {
            if (_state[i] != 0 && _state[i] < last) {
                last = last;
                btn = i;
            }
        }

        for (int i = 1 + VK_RMENU; i < KEY_TOTAL; i++) {
            if (_state[i] != 0 && _state[i] < last) {
                last = last;
                btn = i;
            }
        }

        return btn;
    }

public:
    constexpr KeyReader()
        :
        _state()
    {}

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
        int last = _last_button();
        int output = last * (_state[last] == 1 || _state[last] > CYCLES_HELD);

        return Key(
            (bool)_modifier_ticks(VK_LSHIFT, VK_RSHIFT),
            (bool)_modifier_ticks(VK_LCONTROL, VK_RCONTROL),
            (bool)_modifier_ticks(VK_LMENU, VK_RMENU),
            (bool)_modifier_ticks(VK_LWIN, VK_RWIN),
            output,
            _state[last] > CYCLES_HELD,
            '\0'
        );
    }

    int* get_states() {
        return _state;
    }
};
