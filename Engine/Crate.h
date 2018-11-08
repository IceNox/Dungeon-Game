#pragma once

#include "Tile.h"

#include <string>
#include <vector>
#include <chrono>

class Crate
{
public:
	int gX;
	int gY;

	int cX;
	int cY;

	int sX;
	int sY;

	int spriteOffsetX = -36;
	int spriteOffsetY = -68;

	bool revealed;
	bool visible;

	bool onGround;

	float jumpHeight = 25.0f;

	bool changedPosition;

	float MAlength = 0.15f;
	bool inMA;
	float MAtimeElapsed;
	float MAprogress;
	std::string MAdirection;
	std::chrono::time_point<std::chrono::system_clock> MAstart;
	int sgX;
	int sgY;

	Crate(int gX, int gY);
	void update_crate();
	void damage(const std::vector<Tile> &tiles, std::string direction);
};