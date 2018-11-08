#pragma once

#include "Command.h"

#include "HealthMap.h"
#include "Sprite.h"
#include "Colors.h"

#include <vector>
#include <chrono>

using namespace std::chrono;

class AI
{
public:
	int health;
	int armor;
	int maxHealth;

	int healthbarHeight;

	float resistancePhysical;
	float resistanceFire;
	float resistanceWater;
	float resistanceMagic;

	bool immunePhysical;
	bool immuneFire;
	bool immuneWater;
	bool immuneMagic;

	int gX;
	int gY;
	
	int cX;
	int cY;

	int spriteOffsetX;
	int spriteOffsetY;

	int sX;
	int sY;

	int hitboxRadius;

	int gold;

	bool floating;
	bool onGround;

	bool active = false;

	bool revealed		  = false;
	bool visible		  = false;
	bool healthbarVisible = false;
	bool hitboxActive	  = true;

	float damageCooldown = 0.5f;
	time_point<system_clock> lastTookDamage;

	Color deathParticleColor;

	bool  spawning	  = false;
	float spawnLength = 0.0f;
	time_point<system_clock> spawnStart;

	bool targetable = true;
	bool dead		= false;
	
	void damage
	(
		int amount,
		std::string type,
		Direction direction,
		std::vector<Command> &commands,
		bool identificator
	)
	{
		DamageMap dm;
		dm.direction = direction;

		if		(type == "physical") dm.playerDamagePhysical += amount;
		else if (type == "fire")	 dm.playerDamageFire += amount;
		else if (type == "water")	 dm.playerDamageWater += amount;
		else if (type == "magic")	 dm.playerDamageMagic += amount;

		damage(dm, commands, true);
	}
	
	void damage
	(
		DamageMap damageMap,
		std::vector<Command> &commands,
		bool identificator
	)
	{
		using namespace std::chrono;

		duration<float> elapsed_time = system_clock::now() - lastTookDamage;
		if (elapsed_time < microseconds((int)(damageCooldown * 1000000))) return;

		int amount = 0;

		int amountP = damageMap.enemyDamagePhysical;
		int amountF = damageMap.enemyDamageFire;
		int amountW = damageMap.enemyDamageWater;
		int amountM = damageMap.enemyDamageMagic;

		if (onGround) {
			amountP += damageMap.enemyDamagePhysicalGround;
			amountF += damageMap.enemyDamageFireGround;
			amountW += damageMap.enemyDamageWaterGround;
			amountM += damageMap.enemyDamageMagicGround;
		}

		if (amountP > 0) {
			amountP -= ceil(resistancePhysical);
			if (amountP < 1) amountP = 1;

			if (immunePhysical) amountP = 0;
		}
		if (amountF > 0) {
			amountF -= ceil(resistanceFire);
			if (amountF < 1) amountF = 1;

			if (immuneFire) amountF = 0;
		}
		if (amountW > 0) {
			amountW -= ceil(resistanceWater);
			if (amountW < 1) amountW = 1;

			if (immuneWater) amountW = 0;
		}
		if (amountM > 0) {
			amountM -= ceil(resistanceMagic);
			if (amountM < 1) amountM = 1;

			if (immuneMagic) amountM = 0;
		}

		amount += amountP + amountF + amountW + amountM;

		if (amount == 0) return;

		healthbarVisible = true;

		if (armor > 0) {
			if (armor - amount < 0) {
				amount -= armor;
				armor = 0;
				health -= amount;
				return;
			}
			else {
				armor -= amount;
				return;
			}
		}

		health -= amount;
		if (health < 0) health = 0;
		if (health == 0) {
			healthbarVisible = false;
			dead = true;

			// Drop gold
			if (gold > 0) commands.push_back(Command(gX, gY, gold));

			// Spawn death particles
			Sprite particle = create_rectangle(8, 8, deathParticleColor);

			if (damageMap.direction == NO_DIRECTION) damageMap.direction == UP;

			if (damageMap.direction == UP) {
				commands.push_back(Command(cX + 4, cY - 60, particle, 5, -800, -5, 800, 0.25f, 0.25f));
				commands.push_back(Command(cX + 8, cY - 52, particle, 100, -600, -100, 600, 0.25f, 0.25f));
				commands.push_back(Command(cX - 8, cY - 56, particle, -50, -600, 50, 600, 0.25f, 0.25f));
				commands.push_back(Command(cX - 8, cY - 44, particle, -100, -400, 100, 400, 0.25f, 0.25f));
			}
			else if (damageMap.direction == DOWN) {
				commands.push_back(Command(cX + 4, cY - 66, particle, 5, 800, -5, -800, 0.25f, 0.25f));
				commands.push_back(Command(cX + 8, cY - 52, particle, 100, 600, -100, -600, 0.25f, 0.25f));
				commands.push_back(Command(cX - 8, cY - 56, particle, -50, 600, 50, -600, 0.25f, 0.25f));
				commands.push_back(Command(cX - 8, cY - 44, particle, -100, 400, 100, -400, 0.25f, 0.25f));
			}
			else if (damageMap.direction == LEFT) {
				commands.push_back(Command(cX + 4, cY - 66, particle, -400, -100, 400, 100, 0.25f, 0.25f));
				commands.push_back(Command(cX + 8, cY - 52, particle, -300, -100, 300, 100, 0.25f, 0.25f));
				commands.push_back(Command(cX - 8, cY - 56, particle, -600, 100, 600, 100, 0.25f, 0.25f));
				commands.push_back(Command(cX - 8, cY - 44, particle, -500, 200, 500, 100, 0.25f, 0.25f));
			}
			else if (damageMap.direction == RIGHT) {
				commands.push_back(Command(cX + 4, cY - 66, particle, 600, -100, -600, 100, 0.25f, 0.25f));
				commands.push_back(Command(cX + 8, cY - 52, particle, 500, -100, -500, 100, 0.25f, 0.25f));
				commands.push_back(Command(cX - 8, cY - 56, particle, 400, 100, -400, 100, 0.25f, 0.25f));
				commands.push_back(Command(cX - 8, cY - 44, particle, 300, 200, -300, 100, 0.25f, 0.25f));
			}
		}
	}
};