#pragma once

#include "Constants.h"
#include "Position.h"
#include "GameTime.h"

#include <string>
#include <vector>

struct GameStateData
{
	// Variables for starting a game
	bool inLevel	 = false;
	bool levelLoaded = false;
	bool enterLevel	 = false;
	LevelType	lType;
	std::string	lName;

	// Variables for loading the editor
	bool inEditor	  = false;
	bool editorLoaded = false;
	bool enterEditor  = false;

	// Variables for entering menu
	bool enterMenu = false;
};

struct ActionState
{
	bool ongoing;
	
	int	  duration;
	float progress;

	TimePoint start;
};

struct Hitbox
{
	int type = 0; // 0 - circle, 1 - rectangle

	Pos2D cPos;
	int	width  = 0;
	int height = 0;
	int radius = 0;

	bool active = true;
};