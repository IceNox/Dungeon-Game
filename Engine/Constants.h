#pragma once

#include <string>

// Path data
const std::string CONFIG_PATH = "Content/User/Config.json";
const std::string SAVE_PATH = "Content/Saves/";

// Cell data
const int cellWidth = 96;
const int cellHeight = 96;
const int cellSize = 96;

const int levelsInStage = 6;

enum MenuElementType
{
    ME_BUTTON,
    ME_TEXT,
    ME_SLIDER,
    ME_SWITCH,
    ME_SELECTION,
    ME_PICTURE
};

enum MenuSelectionMarkerType
{
    SM_LINE,
    SM_ARROW
};

enum MenuSceneName
{
    SCENE_MAIN_MENU,
    SCENE_PLAY,
    SCENE_SAVES,
    SCENE_USER_LEVELS,
    SCENE_OPTIONS,
    SCENE_VIDEO,
    SCENE_CONTROLS
};

enum DamageType
{
    D_PHYSICAL,
    D_AIR,
    D_EARTH,
    D_FIRE,
    D_WATER,
    D_HOLY,
    D_DEMONIC
};

enum MobName
{
    M_BARBARIAN,
    M_WIZARD,
    M_PALADIN,
    M_ZOMBIE,
    M_LICH,
    M_WRAITH
};

enum MobType
{
    M_SMALL,
    M_MEDIUM,
    M_LARGE,
    M_BOSS
};

enum MobAction
{
    M_IDLE,
    M_MOVING,
    M_ATTACKING,
    M_CASTING_BOLT,
    M_CASTING_ERUPTION,
    M_CASTING_SLOW,
    M_CHARGING,
    M_BLOCKING,
    M_RAISING_ZOMBIE
};

enum PlayerAction
{
    P_IDLE,
    P_MOVING
};

enum TileType
{
    WALL,
    BASE_FLOOR,
    FLOOR,
    WATER,
    LAVA
};

enum TileVariant
{
    V_STONE,
    V_DIRT
};

enum LevelType
{
    NEW_GAME,
    SAVE,
    USER
};

enum SpriteName
{
    // Blank sprite
    EMPTY_SPRITE,

    // Blocks
    DIRT,
    STONE,
    STONE_SIDE,
    EXIT_GREEN,
    WATER_1,
    WATER_2,
    WATER_3,
    LAVA_1,
    LAVA_2,
    LAVA_3,

    // Entities
    WIZARD_BOLT_UP,
    WIZARD_BOLT_DOWN,
    WIZARD_BOLT_LEFT,
    WIZARD_BOLT_RIGHT,

    LICH_BOLT_UP,
    LICH_BOLT_DOWN,
    LICH_BOLT_LEFT,
    LICH_BOLT_RIGHT,

    GOLD_1,
    GOLD_2,
    GOLD_3,
    GOLD_4,
    GOLD_5,
    GOLD_6,
    GOLD_7,

    // Player
    PLAYER_IDLE_LEFT_1,
    PLAYER_IDLE_LEFT_2,
    PLAYER_IDLE_RIGHT_1,
    PLAYER_IDLE_RIGHT_2,
    PLAYER_JUMP_LEFT_1,
    PLAYER_JUMP_LEFT_2,
    PLAYER_JUMP_RIGHT_1,
    PLAYER_JUMP_RIGHT_2,

    // UI
    SHIELD_SMALL_HALF,
    SHIELD_SMALL_FULL,
    HEART_SMALL_EMPTY,
    HEART_SMALL_HALF,
    HEART_SMALL_FULL,
    SHIELD_HALF,
    SHIELD_FULL,
    HEART_EMPTY,
    HEART_HALF,
    HEART_FULL,
    ITEM_FRAME,
    ITEM_FRAME_SELECTED,
    GOLD_COIN,

    // Other
    EFFECT_SLOWED,
    EFFECT_STUNNED,
    EFFECT_BURNING,

    // Elements
    SELECTION_MARKER,
    SELECTION_ARROW_LEFT,
    SELECTION_ARROW_RIGHT,
    SLIDER_BODY,
    SLIDER_HEAD,

    // Animations
    ATTACK_CHOP_UP_ANIMATION,
    ATTACK_CHOP_DOWN_ANIMATION,
    ATTACK_CHOP_LEFT_ANIMATION,
    ATTACK_CHOP_RIGHT_ANIMATION,
    ERUPTION_WINDUP_ANIMATION,
    ERUPTION_ANIMATION,
    ZOMBIE_SMALL_SPAWN_ANIMATION
};
