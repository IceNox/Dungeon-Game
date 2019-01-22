#pragma once

#include "ChiliWin.h"

#include <bitset>
#include <string>
#include <unordered_map>
#include <vector>

// Definitions
#define CLICKED false
#define HELD true
#define PARENT true

// Specials
const int KEY_TOTAL = 256;
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

// Key names
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
    { VK_LWIN     , "left win"      },
    { VK_RWIN     , "right win"     },
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

class Key {
private:
    enum ModifierKeys { SHIFT, CTRL, ALT, _SIZE_ };
    std::bitset<ModifierKeys::_SIZE_> _modifier_keys;
    int _key = 0;
    bool _held = false;
    char _c = '\0';
    char _c_parent = '\0';

public:
    constexpr Key() {}

    constexpr Key(int key)
        :
        _key(key)
    {}

    constexpr Key(int key, bool click)
        :
        _key(key),
        _held(click)
    {}

    constexpr Key(int key, char c)
        :
        _key(key),
        _c(c)
    {}

    constexpr Key(int key, bool click, char c)
        :
        _key(key),
        _held(click),
        _c(c)
    {}

    // Not a constexpr because the integer could've been read from a file
    Key(bool shift, bool ctrl, bool alt, int key, bool click, char c, char c_parent)
        :
        _key(key),
        _held(click),
        _c(c),
        _c_parent(c_parent)
    {
        _modifier_keys[SHIFT] = shift;
        _modifier_keys[CTRL] = ctrl;
        _modifier_keys[ALT] = alt;
    }

    bool operator== (const Key& key) const
    {
        return (_modifier_keys == key._modifier_keys && _key == key._key && (!_held || key._held));
    }

    bool operator!= (const Key& key) const
    {
        return !(*this == key);
    }

    void operator= (std::vector<int> key)
    {
        _modifier_keys[SHIFT] = (bool)key[0];
        _modifier_keys[CTRL] = (bool)key[1];
        _modifier_keys[ALT] = (bool)key[2];
        _key = key[3];
        _held = CLICKED;
        _c = key[4];
        _c_parent = key[5];
    }

    std::vector<int> get_key_array()
    {
        std::vector<int> output;
        output.push_back((int)_modifier_keys[SHIFT]);
        output.push_back((int)_modifier_keys[CTRL]);
        output.push_back((int)_modifier_keys[ALT]);
        output.push_back((int)_key);
        output.push_back((int)_c);
        output.push_back((int)_c_parent);

        return output;
    }

    int get_key_code()
    {
        return _key;
    }

    char get_key_character(bool parent=false)
    {
        if (parent) {
            return _c_parent;
        }
        else {
            return _c;
        }
    }

    std::string get_key_name(bool parent=false)
    {
        std::string DELIMETER = " + ";

        std::string key_name;
        bool delimeter = false;

        if (_modifier_keys[SHIFT]) {
            delimeter = true;
            key_name = key_name + "shift";
        }

        if (_modifier_keys[CTRL]) {
            if (delimeter) {
                key_name = key_name + DELIMETER;
            }

            delimeter = true;
            key_name = key_name + "ctrl";
        }

        if (_modifier_keys[ALT]) {
            if (delimeter) {
                key_name = key_name + DELIMETER;
            }

            delimeter = true;
            key_name = key_name + "alt";
        }

        if (delimeter && (get_key_character(parent) || IS_SPECIAL[_key])) {
            key_name = key_name + DELIMETER;
        }

        if (IS_SPECIAL[_key]) {
            for (std::unordered_map<int, std::string>::const_iterator it = SPECIAL_KEYS.begin(); it != SPECIAL_KEYS.end(); ++it) {
                if (_key == it->first) {
                    key_name = key_name + it->second;
                    break;
                }
            }
        }
        else if (get_key_character(parent)) {
            key_name = key_name + get_key_character(parent);
        }

        return key_name;
    }
};
