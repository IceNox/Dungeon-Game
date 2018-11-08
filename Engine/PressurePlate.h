#pragma once

#include "Tile.h"

#include <string>
#include <vector>

class PressurePlate
{
public:
	int gX;
	int gY;

	int sX;
	int sY;

	std::string type;
	// "wooden" can be activated by items/gold/crates/player/mobs
	// "steel" can be activated by crates/player/mobs
	// "gold" con be activated by gold
	// "blood" con be activated by enemies

	bool pressed;

	bool revealed;
	bool visible;

	PressurePlate(int gX, int gY, std::string type);
	void update_plate(std::vector<Tile> &tiles);
};