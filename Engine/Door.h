#pragma once

#include "Command.h"

#include "HealthMap.h"
#include "Particle.h"

#include <string>
#include <vector>

class Door
{
public:
	int gX;
	int gY;

	int cX;
	int cY;

	int sX;
	int sY;

	std::string facing;

	bool revealed;
	bool visible;

	Door
	(
		int gX,
		int gY,
		std::string facing
	);

	void damage
	(
		DamageMap damageMap,
		std::vector<Command> &commands
	);
};