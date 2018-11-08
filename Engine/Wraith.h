#pragma once

#include "AI.h"
#include "Tile.h"
#include "Player.h"
#include "HealthMap.h"

#include "Command.h"

#include <string>
#include <chrono>
#include <vector>

class Wraith : public AI
{
public:
	std::string type;
	std::string facing;

	std::string currentAction;
	std::string spriteFacing;
	std::string spriteNumber;

	int explosionDamage;

	float actionCooldown = 1.0f;
	time_point<system_clock> lastAction;

	// Movement animation variables
	bool changedPosition = false;

	float					 MAlength = 0.2f;
	float					 c_MAlength;
	bool					 inMA = false;
	float					 MAtimeElapsed;
	float					 MAprogress;
	std::string				 MAdirection;
	time_point<system_clock> MAstart;
	int						 sgX;
	int						 sgY;

	Wraith
	(
		int gX,
		int gY,
		std::string type,
		std::vector<Command> &commands,
		bool spawn = false
	);

private:
	void spawn();

public:
	void update_wraith
	(
		Player &player,
		std::vector<Tile> &tiles,
		std::vector<Command> &commands
	);

	void explode
	(
		Player &player,
		std::vector<Command> &commands
	);

	void move
	(
		Player &player,
		std::vector<Tile> &tiles
	);

	void end_MA();

	void damage
	(
		int amount,
		std::string type,
		Direction direction,
		std::vector<Command> &commands
	);

	void damage
	(
		DamageMap damageMap,
		std::vector<Command> &commands
	);
};