#pragma once

#include "Navigation.h"
#include "Sprite.h"
#include "SolStateArray.h"

#include <string>
#include <vector>

struct ScriptLoadData
{
    int id;
    std::string filePath;
    std::vector<int> spriteIndexes;

    ScriptLoadData() {};
};

extern int _LEVEL_WIDTH;
extern int _LEVEL_HEIGHT;

//extern std::vector<Pos2D> _PLAYER_GRID_POS;

// Redundant data, used to avoid errors
extern int playerGridX;
extern int playerGridY;

extern int levelWidth;
extern int levelHeight;
//

extern int _PXL_RATIO;

extern std::vector<std::string> _SAVE_FILE_NAMES;
extern std::vector<std::string> _USER_LEVEL_FILE_NAMES;

extern std::vector<Sprite> sprites;
extern std::vector<std::string> spriteFilePaths;

extern SolStateArray scripts;

extern std::vector<ScriptLoadData> _ITEM_SCRIPTS;
extern std::vector<ScriptLoadData> _OBJECT_SCRIPTS;
