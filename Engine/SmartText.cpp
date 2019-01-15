#include "SmartText.h"

#include <string>

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
    _whitelist = SPACES + NUMBERS + LETTERS + SPECIAL;
}

void SmartText::setWhitelist(bool allowSpace, bool allowNumbers, bool allowLetters, bool allowSpecial)
{
    if (allowSpace) {
        _whitelist += SPACES;
    }
    if (allowNumbers) {
        _whitelist += NUMBERS;
    }
    if (allowLetters) {
        _whitelist += LETTERS;
    }
    if (allowSpecial) {
        _whitelist += SPECIAL;
    }
}

void SmartText::setWhitelist(std::string whitelist)
{
    _whitelist = whitelist;
}

void SmartText::update_text(Key &k)
{
    if (_locked) {
        return;
    }

    if ((k == MACRO_BACK) && (_cursor != 0)) {
        _text.erase(--_cursor, 1);
        return;
    }

    if ((k == MACRO_LEFT) && (_cursor != 0)) {
        _cursor--;
        return;
    }

    if ((k == MACRO_RIGHT) && (_cursor != _text.length())) {
        _cursor++;
        return;
    }

    char c = k.get_key_character();
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
