#pragma once

#include "Command.h"

#include "AI.h"
#include "Player.h"
#include "Entity.h"
#include "Particle.h"
#include "HealthMap.h"
#include "Animation.h"

#include <string>
#include <chrono>
#include <vector>

using namespace std::chrono;

class Barbarian : public AI
{
public:
	std::string type;
	std::string facing;

	std::string currentAction;
	std::string spriteFacing;
	std::string spriteNumber;

	int hitDamage;

	float actionCooldown = 1.0f;
	time_point<system_clock> lastAction;

	// Movement animation variables
	float jumpHeight = 30.0f;

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

	// Attack animation variables
	float					 AAlength = 0.5f;
	bool					 inAA = false;
	float					 AAtimeElapsed;
	float					 AAprogress;
	std::string				 AAdirection;
	time_point<system_clock> AAstart;

	Barbarian
	(
		int gX,
		int gY,
		std::string type,
		std::vector<Command> &commands,
		bool spawn = false
	);

	void update_barbarian
	(
		Player &player,
		std::vector<Tile> &tiles,
		std::vector<DamageMap> &damageMap,
		std::vector<Command> &commands
	);

	void attack
	(
		Player &player,
		std::vector<DamageMap> &damageMap,
		std::vector<Command> &commands
	);
	
	void move
	(
		std::vector<Tile> &tiles,
		Player &player
	);

	void end_MA();
	
	void update_idle_animation();

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