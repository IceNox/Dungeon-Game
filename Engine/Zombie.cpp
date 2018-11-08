#include "Zombie.h"

#include "GlobalData.h"
#include "Constants.h"
#include "Functions.h"

Zombie::Zombie(
	int			gX,
	int			gY,
	std::string type,
	std::vector<Command> &commands,
	bool		spawn
)
{
	using namespace std::chrono;

	this->gX   = gX;
	this->gY   = gY;
	this->type = type;

	facing = "left";

	currentAction = "idle";
	spriteFacing  = "left";
	spriteNumber  = "1";

	cX = gX * cellSize + cellSize / 2;
	cY = gY * cellSize + cellSize / 2;

	floating = false;
	onGround = true;

	if (type == "small") {
		health			= 2;
		armor			= 0;
		maxHealth		= 2;
		hitDamage		= 2;
		healthbarHeight = 78;
		spriteOffsetX	= -32;
		spriteOffsetY	= -88;
		hitboxRadius	= 16;
		gold			= 20;

		resistancePhysical = 0.0f;
		resistanceFire	   = 0.0f;
		resistanceWater	   = 0.0f;
		resistanceMagic    = 0.0f;

		immunePhysical	= false;
		immuneFire		= false;
		immuneWater		= false;
		immuneMagic		= false;

		deathParticleColor = Color(167, 178, 127);
	}

	sX = cX + spriteOffsetX;
	sY = cY + spriteOffsetY;

	lastAction	   = system_clock::now();
	lastTookDamage = system_clock::now();

	if (spawn) {
		gold = 0;

		visible	   = false;
		targetable = false;

		spawning	= true;
		spawnLength = 0.45f;

		//visualEffects.push_back(VisualEffect(sX, sY, false, "zombie_small_spawn_effect", 64, 96, 0.05f, 9));
		commands.push_back(Command(sX, sY, false, "zombie_small_spawn_effect", 64, 96, 0.05f, 9, 1.0f));

		spawnStart = system_clock::now();
	}
}

void Zombie::update_zombie
(
	Player &player,
	std::vector<Tile> &tiles,
	std::vector<DamageMap> &damageMap,
	std::vector<Command> &commands,
	std::vector<Zombie> &zombies
)
{
	if (spawning) {
		spawn();
		return;
	}
	if (!active)  return;

	update_ACDMP(player, zombies);
	attack(player, damageMap, commands);
	move(player, tiles);
	update_idle_animation();
}

void Zombie::spawn()
{
	duration<float> elapsed_time = system_clock::now() - spawnStart;

	float secondsElapsed = elapsed_time.count();
	float progress		 = secondsElapsed / spawnLength;
	
	if (progress >= 0.95f) {
		spawning = false;

		targetable = true;

		lastAction = system_clock::now() - milliseconds(500);
	}
}

void Zombie::attack
(
	Player &player,
	std::vector<DamageMap> &damageMap,
	std::vector<Command> &commands
)
{
	using namespace std::chrono;

	if (!inMA && !inAA) {
		duration<float> elapsed_time = system_clock::now() - lastAction;
		if (elapsed_time < microseconds((int)(actionCooldown * 1000000))) return;

		std::string enemyDirection = "";

		if		(player.gX == gX	 && player.gY == gY - 1) enemyDirection = "up";
		else if (player.gX == gX	 && player.gY == gY + 1) enemyDirection = "down";
		else if (player.gX == gX - 1 && player.gY == gY)	 enemyDirection = "left";
		else if (player.gX == gX + 1 && player.gY == gY)	 enemyDirection = "right";

		if (enemyDirection != "") {
			currentAction = "attack";
			spriteNumber  = "1";

			inAA		= true;
			AAdirection = enemyDirection;
			AAstart		= system_clock::now();

			facing = enemyDirection;
			if		(facing == "left")  spriteFacing = "left";
			else if (facing == "right") spriteFacing = "right";
		}
	}
	else if (!inMA && inAA) {
		// Update time
		duration<float> elapsed_seconds = system_clock::now() - AAstart;

		AAtimeElapsed = elapsed_seconds.count();
		AAprogress	  = AAtimeElapsed / AAlength;

		// End AA if nescessary
		if (AAprogress >= 1.0f) {
			inAA		  = false;
			currentAction = "idle";
			spriteNumber  = "1";

			actionCooldown = 0.25f;
			lastAction = system_clock::now();

			int index;
			if		(AAdirection == "up")	 index = (gY - 1) * levelWidth + gX;
			else if (AAdirection == "down")  index = (gY + 1) * levelWidth + gX;
			else if (AAdirection == "left")  index = gY	* levelWidth + gX - 1;
			else if (AAdirection == "right") index = gY	* levelWidth + gX + 1;

			damageMap[index].active				  = true;
			damageMap[index].direction			  = NO_DIRECTION;
			damageMap[index].playerDamagePhysical += hitDamage;

			int soX;
			int soY;

			int sprX;
			int sprY = 72;

			if		(AAdirection == "up") {
				soX = 36;
				soY = -48;

				sprX = 8;
			}
			else if (AAdirection == "down") {
				soX = 28;
				soY = 48;

				sprX = 8;
			}
			else if (AAdirection == "left") {
				soX = -40;
				soY = 12;

				sprX = 24;
			}
			else if (AAdirection == "right") {
				soX = 88;
				soY = 12;

				sprX = 24;
			}

			//visualEffects.push_back(VisualEffect(sX + soX, sY + soY, false, "attack_chop_" + AAdirection + "_effect", sprX, sprY, 0.02f, 7));
			commands.push_back(Command(sX + soX, sY + soY, false, "attack_chop_" + AAdirection + "_effect", sprX, sprY, 0.02f, 7, 1.0f));
		}
	}
}

void Zombie::move
(
	Player &player,
	std::vector<Tile> &tiles
)
{
	using namespace std::chrono;

	if (!inMA && !inAA) {
		duration<float> elapsed_time = system_clock::now() - lastAction;
		if (elapsed_time < microseconds((int)(actionCooldown * 1000000))) return;

		std::string direction;

		if		(player.gY == gY) {
			if (player.gX < gX) direction = "left";
			else				direction = "right";
		}
		else if (player.gX == gX) {
			if (player.gY < gY) direction = "up";
			else				direction = "down";
		}
		else {
			if		(facing == "up") {
				if (player.gY < gY) direction = "up";
				else {
					if (player.gX < gX) direction = "left";
					else				direction = "right";
				}
			}
			else if (facing == "down") {
				if (player.gY > gY) direction = "down";
				else {
					if (player.gX < gX) direction = "left";
					else				direction = "right";
				}
			}
			else if (facing == "left") {
				if (player.gX < gX) direction = "left";
				else				direction = "right";
			}
			else if (facing == "right") {
				if (player.gX < gX) direction = "left";
				else				direction = "right";
			}
		}

		if		(direction == "up") {
			if (tiles[(gY - 1) * levelWidth + gX].occupied) {
				if		(player.gX < gX) direction = "left";
				else if (player.gX > gX) direction = "right";
				else					 direction = "up";
			}
		}
		else if (direction == "down") {
			if (tiles[(gY + 1) * levelWidth + gX].occupied) {
				if		(player.gX < gX) direction = "left";
				else if (player.gX > gX) direction = "right";
				else					 direction = "down";
			}
		}
		else if (direction == "left") {
			if (tiles[gY * levelWidth + gX - 1].occupied) {
				if (player.gY < gY) direction = "up";
				else if (player.gY > gY) direction = "down";
				else					 direction = "left";
			}
		}
		else if (direction == "right") {
			if (tiles[gY * levelWidth + gX + 1].occupied) {
				if (player.gY < gY) direction = "up";
				else if (player.gY > gY) direction = "down";
				else					 direction = "right";
			}
		}

		if (direction == "up"    && tiles[(gY - 1) * levelWidth + gX].occupied) return;
		if (direction == "down"  && tiles[(gY + 1) * levelWidth + gX].occupied) return;
		if (direction == "left"	 && tiles[gY * levelWidth + gX - 1].occupied) return;
		if (direction == "right" && tiles[gY * levelWidth + gX + 1].occupied) return;

		currentAction = "jump";
		spriteNumber  = "1";

		inMA		= true;
		onGround	= false;
		MAdirection = direction;
		MAstart		= system_clock::now();
		sgX = gX;
		sgY = gY;

		c_MAlength = MAlength;

		facing = direction;
		if		(facing == "left")  spriteFacing = "left";
		else if (facing == "right") spriteFacing = "right";

		actionCooldown = baseActionCooldown * ACDMP;
		lastAction = system_clock::now();

		// Set the tile the barbarian is moving to as occupied to prevent other AI / player from entering it
		if		(direction == "up")    tiles[(gY - 1) * levelWidth + gX].occupied = true;
		else if (direction == "down")  tiles[(gY + 1) * levelWidth + gX].occupied = true;
		else if (direction == "left")  tiles[gY * levelWidth + gX - 1].occupied = true;
		else if (direction == "right") tiles[gY * levelWidth + gX + 1].occupied = true;
	}
	else if (!inAA && inMA) {
		std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - MAstart;

		MAtimeElapsed = elapsed_seconds.count();
		MAprogress	  = MAtimeElapsed / c_MAlength;

		// Calculate distance moved while in the MA
		int movedBy;
		movedBy = cellSize - cellSize * (1.0f - MAprogress) * (1.0f - MAprogress);

		// If the barbarian moved halfway to another tile, change his coordinates
		if (MAprogress > 0.3f && changedPosition == false) {
			changedPosition = true;

			if		(MAdirection == "up") {
				gY--;
			}
			else if (MAdirection == "down") {
				gY++;
			}
			else if (MAdirection == "left") {
				gX--;
			}
			else if (MAdirection == "right") {
				gX++;
			}
		}

		// Set correct sprites, depending on MA progress
		if (MAprogress < 1.0f) spriteNumber = "1";

		// Calculate current height
		int height = 10;
		if (!floating) {
			float xAxis = 2 * (MAprogress - 0.5f);
			float yAxis = xAxis * xAxis * -1.0f;

			height = ceil(yAxis * jumpHeight) + (int)jumpHeight + 1;
		}

		// End MA if the progress is 100%
		if (MAprogress >= 1.0f) {
			end_MA();
		}

		// Otherwise, update barbarian position
		else {
			if		(MAdirection == "up") {
				cY = sgY * cellHeight + cellHeight / 2 - movedBy;
			}
			else if (MAdirection == "down") {
				cY = sgY * cellHeight + cellHeight / 2 + movedBy;
			}
			else if (MAdirection == "left") {
				cX = sgX * cellWidth + cellWidth / 2 - movedBy;
			}
			else if (MAdirection == "right") {
				cX = sgX * cellWidth + cellWidth / 2 + movedBy;
			}
		}

		// Update barbarian sprite position
		sX = cX + spriteOffsetX;
		sY = cY + spriteOffsetY;

		if (inMA) sY -= height;
	}
}

void Zombie::end_MA()
{
	inMA			= false;
	onGround		= true;
	changedPosition = false;
	currentAction	= "idle";
	spriteNumber	= "1";

	cX = gX * cellSize + cellSize / 2;
	cY = gY * cellSize + cellSize / 2;
}

void Zombie::update_idle_animation()
{
	if (!inMA && !inAA) {
		duration<float> elapsed_time = system_clock::now() - lastAction;
		if ((int)(elapsed_time.count() / (1.0f / 2)) % 2 == 0) {
			spriteNumber = "1";
		}
		else {
			spriteNumber = "2";
		}
	}
}

void Zombie::update_ACDMP
(
	Player &player,
	std::vector<Zombie> &zombies
)
{
	int dist = distance(Pos2D(gX, gY), Pos2D(player.gX, player.gY)) - 1;

	// Calculate multiplier
	ACDMP = 1.0f + 0.5f * dist;

	// For every nearby zombie besides this one reduce action cooldown by .25s
	for (int i = 0; i < zombies.size(); i++) {
		int d = distance(Pos2D(gX, gY), Pos2D(zombies[i].gX, zombies[i].gY));

		if (d < 5 && d > 0) ACDMP -= 0.5f;
	}

	// Apply limits
	if (ACDMP > 4.0f) ACDMP = 4.0f;
	if (ACDMP < 0.5f) ACDMP = 0.5f;
}

void Zombie::damage
(
	int amount,
	std::string type,
	Direction direction,
	std::vector<Command> &commands
)
{
	AI::damage(amount, type, direction, commands, true);
}

void Zombie::damage
(
	DamageMap damageMap,
	std::vector<Command> &commands
)
{
	AI::damage(damageMap, commands, true);
}