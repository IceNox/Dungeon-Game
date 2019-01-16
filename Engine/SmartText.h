#pragma once

#include "Key.h"

// Constants
const int TEXT_RESERVE = 30;

// SmartText class implementation
const std::string SPACES  = " ";
const std::string NUMBERS = "0123456789";
const std::string LETTERS = "abcdefghijklmnopqrstuvwxyz";
const std::string SPECIAL = "\\,./#;[]`-=+*";

// Macros
const Key MACRO_BACK  = Key(VK_BACK  , HELD);
const Key MACRO_LEFT  = Key(VK_LEFT  , HELD);
const Key MACRO_RIGHT = Key(VK_RIGHT , HELD);

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
    void update_text(Key &k);
    void clear_text();
    void set_text(std::string text);
    void clear_prefix();
    void set_prefix(std::string prefix);
    void toggle_lock();
};
