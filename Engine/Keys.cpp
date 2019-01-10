#include "Keys.h"

namespace kb
{
    // Helper functions for converting between indices, codes, names and characters
    int index_to_code(int index)
    {
        return keyCodes[index];
    }

    char index_to_char(int index)
    {
        return keyCharacters[index];
    }

    std::string index_to_name(int index)
    {
        return keyNames[index];
    }

    int code_to_index(int code)
    {
        for (int i = 0; i < KEY_COUNT; i++) {
            if (keyCodes[i] == code) {
                return i;
            }
        }
        return -1;
    }

    char code_to_char(int code)
    {
        for (int i = 0; i < KEY_COUNT; i++) {
            if (keyCodes[i] == code) {
                return keyCharacters[i];
            }
        }
        return 0;
    }

    std::string code_to_name(int code)
    {
        for (int i = 0; i < KEY_COUNT; i++) {
            if (keyCodes[i] == code) {
                return keyNames[i];
            }
        }
        return "unknown";
    }

    int name_to_code(std::string name)
    {
        for (int i = 0; i < KEY_COUNT; i++) {
            if (keyNames[i] == name) {
                return keyCodes[i];
            }
        }
        return -1;
    }

    // SmartText class implementation
    const std::string SmartText::spaces  = " ";
    const std::string SmartText::numbers = "0123456789";
    const std::string SmartText::letters = "abcdefghijklmnopqrstuvwxyz";
    const std::string SmartText::special = "";

    SmartText::SmartText()
        :
        _text(),
        _prefix(),
        _whitelist(),
        _cursor(),
        _locked(false)
    {
        _text.reserve(TEXT_RESERVE);
    }

    SmartText::SmartText(std::string prefix)
        :
        _text(),
        _prefix(prefix),
        _whitelist(),
        _cursor(),
        _locked(false)
    {
        _text.reserve(TEXT_RESERVE);
    }

    SmartText::SmartText(std::string text, std::string prefix)
        :
        _text(text),
        _prefix(prefix),
        _whitelist(),
        _cursor(text.length()),
        _locked(false)
    {
        _text.reserve(TEXT_RESERVE);
    }

    std::string SmartText::get_text()
    {
        return _text;
    }

    std::string SmartText::get_prefix()
    {
        return _prefix;
    }

    std::string SmartText::get_content()
    {
        return get_prefix() + get_text();
    }

    void SmartText::setWhitelist()
    {
        _whitelist = spaces + numbers + letters + special;
    }

    void SmartText::setWhitelist(bool allowSpace, bool allowNumbers, bool allowLetters, bool allowSpecial)
    {
        if (allowSpace) {
            _whitelist += spaces;
        }
        if (allowNumbers) {
            _whitelist += numbers;
        }
        if (allowLetters) {
            _whitelist += letters;
        }
        if (allowSpecial) {
            _whitelist += special;
        }
    }

    void SmartText::setWhitelist(std::string whitelist)
    {
        _whitelist = whitelist;
    }

    void SmartText::update_text(Keys &k)
    {
        int time_pressed = MAX_INT;
        int index = 0;

        if (_locked) {
            return;
        }

        if ((k.key_clicked(KC_BACKSPACE) || k.key_held(KC_BACKSPACE)) && (_cursor != 0)) {
            _text.erase(--_cursor, 1);
            return;
        }

        if ((k.key_clicked(KC_LEFT_ARROW) || k.key_held(KC_LEFT_ARROW)) && (_cursor != 0)) {
            _cursor--;
            return;
        }

        if ((k.key_clicked(KC_RIGHT_ARROW) || k.key_held(KC_RIGHT_ARROW)) && (_cursor != _text.length())) {
            _cursor++;
            return;
        }


        for (int i = 0; i < KEY_COUNT; i++) {
            if (k.index_state(i)) {
                if (k.index_state(i) < time_pressed) {
                    time_pressed = k.index_state(i);
                    index = i;
                }
            }
        }

        if (!k.index_clicked(index) && !k.index_held(index)) {
            return;
        }

        char c = index_to_char(index);
        if (_whitelist.find(c) != std::string::npos) {
            _text.insert(_cursor, 1, c);
            _cursor++;
        }
    }

    void SmartText::clear_text()
    {
        _text.clear();
        _cursor = 0;
    }

    void SmartText::set_text(std::string text)
    {
        _text = text;
        _cursor = text.length();
    }

    void SmartText::clear_prefix()
    {
        _prefix.clear();
    }

    void SmartText::set_prefix(std::string prefix)
    {
        _prefix = prefix;
    }

    void SmartText::toggle_lock()
    {
        if (_locked) {
            _locked = false;
        }
        else {
            _locked = true;
        }
    }
}
