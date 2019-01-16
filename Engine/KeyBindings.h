#pragma once

#include "Key.h"

#include <string>

class KeyBindings
{
public:
    /// Default virtual-key codes

    // Menu
    const Key UP     = Key(VK_UP     , HELD);
    const Key DOWN   = Key(VK_DOWN   , HELD);
    const Key LEFT   = Key(VK_LEFT   , HELD);
    const Key RIGHT  = Key(VK_RIGHT  , HELD);
    const Key ENTER  = Key(VK_RETURN , CLICKED);
    const Key ESCAPE = Key(VK_ESCAPE , CLICKED);

    // Game
    Key MOVE_UP          = Key(VK_UP    , CLICKED);
    Key MOVE_DOWN        = Key(VK_DOWN  , CLICKED);
    Key MOVE_LEFT        = Key(VK_LEFT  , CLICKED);
    Key MOVE_RIGHT       = Key(VK_RIGHT , CLICKED);
    Key USE_ITEM         = Key(VK_SPACE , CLICKED);
    Key DROP_ITEM        = Key(VK_MENU  , CLICKED);
    Key CHANGE_DIRECTION = Key(VK_SHIFT , CLICKED);
    Key PAUSE            = Key((int)'P' , CLICKED);
    Key CHANGE_MAP_SIZE  = Key((int)'M' , CLICKED);
    Key ITEM_1           = Key((int)'1' , CLICKED);
    Key ITEM_2           = Key((int)'2' , CLICKED);
    Key ITEM_3           = Key((int)'3' , CLICKED);
    Key ITEM_4           = Key((int)'4' , CLICKED);
    Key ITEM_5           = Key((int)'5' , CLICKED);

    KeyBindings() {};
};
