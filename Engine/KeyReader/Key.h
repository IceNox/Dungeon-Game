#include <bitset>

class Key {
private:
    enum ModifierKeys { SHIFT, CTRL, ALT, WIN, _SIZE_ };
    std::bitset<ModifierKeys::_SIZE_> _modifierKeys;
    int _key = 0;
    char _c = 0;

public:
    constexpr Key() {}

    constexpr Key(bool shift, bool ctrl, bool alt, bool win, int key, char c)
        : _key(key), _c(c)
    {
        _modifierKeys[SHIFT] = shift;
        _modifierKeys[CTRL]  = ctrl;
        _modifierKeys[ALT]   = alt;
        _modifierKeys[WIN]   = win;
    }

    bool operator== (const Key& key) const
    {
        return (_modifierKeys == key._modifierKeys && _key == key._key && _c == key._c);
    }

    bool operator!= (const Key& key) const
    {
        return !(*this == key);
    }
};
