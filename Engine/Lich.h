#pragma once

#include "AI.h"
#include "Player.h"
#include "Zombie.h"
#include "Entity.h"
#include "Particle.h"
#include "HealthMap.h"
#include "Animation.h"

#include "Command.h"

#include <string>
#include <chrono>
#include <vector>

using namespace std::chrono;

class Lich : public AI
{
public:
	std::string type;
	std::string facing;

	std::string currentAction;
	std::string spriteFacing;
	std::string spriteNumber;

	int boltDamage;

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

/// SPELLS
	// Bolt
	float					 boltCooldown = 5.0f;
	float					 BAlength = 0.5f;
	bool					 inBA = false;
	float					 BAtimeElapsed;
	float					 BAprogress;
	std::string				 BAdirection;
	time_point<system_clock> BAstart;
	time_point<system_clock> BAlastCast;

	// Spawn zombie
	float					 spawnCooldown = 5.0f;
	float					 SAlength = 0.5f;
	bool					 inSA = false;
	float					 SAtimeElapsed;
	float					 SAprogress;
	std::string				 SAdirection;
	time_point<system_clock> SAstart;
	time_point<system_clock> SAlastCast;

	Lich
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
	void update_lich
	(
		Player &player,
		std::vector<Tile> &tiles,
		std::vector<DamageMap> &damageMap,
		std::vector<Command> &commands
	);

	void cast_bolt
	(
		Player &player,
		std::vector<Tile> &tiles,
		std::vector<Command> &commands
	);

	void spawn_zombie
	(
		Player &player,
		std::vector<Tile> &tiles,
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