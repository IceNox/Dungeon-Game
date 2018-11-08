#pragma once

#include "Constants.h"

#include "Tile.h"
#include "Item.h"
#include "ScreenAnimations.h"
#include "Animation.h"
#include "StatusEffects.h"

#include "Keys.h"
#include "KeyBindings.h"

#include "UserData.h"

#include <string>
#include <chrono>
#include <vector>

using namespace std::chrono;

class Player
{
public:
    int health;
    int armor;
    int gold;

	int maxHealth;

	float resistancePhysical;
	float resistanceFire;
	float resistanceWater;
	float resistanceMagic;

	int gX, gY, cX, cY, sX, sY;

	Pos2D gPos; // Grid position
	Pos2D cPos; // Pixel position where (0,0) is the top left corner of a map
	Pos2D pPos; // Game pixel position. Game pixel can be a square 4 or 8 pixels in size

	Pos2D spriteOffset;
	Pos2D sPos; // Sprite Position on screen

	int hitboxRadius;

	bool dead;

	bool enterIsPressed  = false;
	bool escapeIsPressed = false;
	bool upIsPressed	 = false;
	bool downIsPressed	 = false;
	bool leftIsPressed	 = false;
	bool rightIsPressed  = false;
	bool spaceIsPressed  = false;

	bool moveBufferUp	 = false;
	bool moveBufferDown  = false;
	bool moveBufferLeft  = false;
	bool moveBufferRight = false;

	int lightLevel = 8;

	bool floating;
	bool onGround;

	float damageCooldown = 0.5f;
	time_point<system_clock> lastTookDamage;

	time_point<system_clock> lastAction;

	// Status effects
	StatusEffects statusEffects;

	// Inventory
	Item items[5];
	int currentlySelectedItem;

	// Sprite variables
	Direction facing;
	std::string skin;

	//Sprite baseDirectionArrowSprite;
	
	Direction spriteFacing;
	int		  spriteNumber;
	
	PlayerAction currentAction;

	SpriteName finalSprite;

	// Variables for shake animation
	bool tookDamage;
	int damageAmount;

    // Movement animation variables
	float jumpHeight = 30.0f; // in pixels

	bool changedPosition = false;

	float MAlength = 5.0f;
	float c_MAlength;
    bool inMA = false;
	float MAtimeElapsed;
	float MAprogress;
	Direction MAdirection;
	time_point<system_clock> MAstart;
	Pos2D sgPos;

	Player
	(
		Pos2D gPos
	);

	void update_player
	(
		std::vector<Tile> tiles,
		std::vector<DamageMap> &damageMap,
		std::vector<Command> &commands,
		ScreenAnimations &screenAnimations,
		Keys &keys,
		UserData &userData,
		bool &paused,
		std::chrono::time_point<std::chrono::system_clock> &pauseStartTime
	);

	void get_input
	(
		Keys	 &keys,
		UserData &userData,
		bool &enter,
		bool &escape,
		bool &up,
		bool &down,
		bool &left,
		bool &right,
		bool &useItem,
		bool &dropItem
	);

	void apply_effects
	(
		std::vector<Command> &commands
	);

	void move
	(
		Keys &keys,
		UserData &userData,
		bool up,
		bool down,
		bool left,
		bool right,
		bool space,
		std::vector<Tile> tiles,
		std::vector<DamageMap>& damageMap,
		std::vector<Command> &commands
	);

	void end_MA();

	void use_items
	(
		bool useItem,
		std::vector<DamageMap>& damageMap,
		std::vector<Command> &commands,
		ScreenAnimations &screenAnimations
	);

	void heal
	(
		int amount
	);

	void damage
	(
		int					 amount,
		DamageType			 type,
		Direction			 direction,
		std::vector<Command> &commands
	);

	void damage
	(
		DamageMap damageMap,
		std::vector<Command> &commands
	);
};
