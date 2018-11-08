#pragma once

#include "Player.h"

#include <string>
#include <vector>

class Sign
{
public:
	int gX;
	int gY;

	int sX;
	int sY;

	std::string rawText;

	std::vector<std::string> textLine;

	bool playerNearby;
	bool revealed;
	bool visible;

	Sign(int gX, int gY, std::string text);

	void update_sign(Player player);
};