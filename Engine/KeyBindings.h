#pragma once

#include "Key.h"

#include <string>

class KeyBindings
{
public:
    /// Default virtual-key codes

    // Menu
    const Key UP     = Key(VK_UP);
    const Key DOWN   = Key(VK_DOWN);
    const Key LEFT   = Key(VK_LEFT);
    const Key RIGHT  = Key(VK_RIGHT);
    const Key ENTER  = Key(VK_RETURN);
    const Key ESCAPE = Key(VK_ESCAPE);

    // Game
    Key MOVE_UP          = Key(VK_UP);
    Key MOVE_DOWN        = Key(VK_DOWN);
    Key MOVE_LEFT        = Key(VK_LEFT);
    Key MOVE_RIGHT       = Key(VK_RIGHT);
    Key USE_ITEM         = Key(VK_SPACE);
    Key DROP_ITEM        = Key(VK_MENU);
    Key CHANGE_DIRECTION = Key(VK_SHIFT);
    Key PAUSE            = Key((int)'P');
    Key CHANGE_MAP_SIZE  = Key((int)'M');
    Key ITEM_1           = Key((int)'1');
    Key ITEM_2           = Key((int)'2');
    Key ITEM_3           = Key((int)'3');
    Key ITEM_4           = Key((int)'4');
    Key ITEM_5           = Key((int)'5');

    KeyBindings() {};
};
