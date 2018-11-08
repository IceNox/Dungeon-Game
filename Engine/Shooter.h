#pragma once

#include "Constants.h"

#include "AI.h"
#include "Entity.h"
#include <string>
#include <chrono>

class Shooter : public AI
{
public:
	std::string type;

	int offset;

	int bulletDamage;
	int bulletSpeed;
	int cooldown;
	int range;
	int bulletHitboxRadius;
	int bulletSpriteOffsetX;
	int bulletSpriteOffsetY;

	std::string facing;

	std::chrono::time_point<std::chrono::system_clock> creationTime;
	std::chrono::time_point<std::chrono::system_clock> lastShotTime;

	Shooter(
		int			gX,
		int			gY,
		std::string type,
		int			offset,
		std::string facing,
		bool		active
	);

	void update_shooter(std::vector<Entity> &entities, int &entityCount);
	void shoot(std::vector<Entity> &entities, int &entityCount);
	void damage(int amount);
};