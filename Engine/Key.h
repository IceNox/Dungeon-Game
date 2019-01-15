#pragma once

#include "ChiliWin.h"

#include <bitset>
#include <string>

class Key {
private:
    enum ModifierKeys { SHIFT, CTRL, ALT, WIN, _SIZE_ };
    std::bitset<ModifierKeys::_SIZE_> _modifier_keys;
    int _key = 0;
    // TODO HELD
    char _c = '\0';
    

public:
    constexpr Key() {}

    Key(int key, char c)
        :
        _key(key),
        _c(c)
    {}

    Key(bool shift, bool ctrl, bool alt, bool win, int key, char c)
        :
        _key(key),
        _c(c)
    {
        _modifier_keys[SHIFT] = shift;
        _modifier_keys[CTRL]  = ctrl;
        _modifier_keys[ALT]   = alt;
        _modifier_keys[WIN]   = win;
    }

    // Not a constexpr because the integer could've been read from a file
    Key(int key)
        :
        _key(key)
    {}

    bool operator== (const Key& key) const
    {
        return (_modifier_keys == key._modifier_keys && _key == key._key && _c == key._c);
    }

    bool operator!= (const Key& key) const
    {
        return !(*this == key);
    }

    void operator= (int key)
    {
        _key = key;
    }

    int get_key_code()
    {
        return _key;
    }

    char get_key_character()
    {
        return _c;
    }

    std::string get_key_name()
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

        if (_modifier_keys[WIN]) {
            if (delimeter) {
                key_name = key_name + DELIMETER;
            }

            delimeter = true;
            key_name = key_name + "win";
        }

        if (_c != '\0') {
            if (delimeter) {
                key_name = key_name + DELIMETER;
            }

            key_name = key_name + _c;
        }

        return key_name;
    }
};
