#include "Wraith.h"

#include "GlobalData.h"
#include "Functions.h"

using namespace std::chrono;

Wraith::Wraith
(
	int gX,
	int gY,
	std::string type,
	std::vector<Command> &commands,
	bool spawn
)
{
	this->gX = gX;
	this->gY = gY;
	this->type = type;

	facing = "left";

	currentAction = "idle";
	spriteFacing = "left";
	spriteNumber = "1";

	cX = gX * cellSize + cellSize / 2;
	cY = gY * cellSize + cellSize / 2;

	floating = true;
	onGround = false;

	if (type == "small") {
		health			= 1;
		armor			= 0;
		maxHealth		= 1;
		healthbarHeight = 110;
		spriteOffsetX	= -24;
		spriteOffsetY	= -100;
		hitboxRadius	= 12;
		explosionDamage = 4;
		gold			= 20;

		resistancePhysical	= 0.0f;
		resistanceFire		= 0.0f;
		resistanceWater		= 0.0f;
		resistanceMagic		= 0.0f;

		immunePhysical	= false;
		immuneFire		= false;
		immuneWater		= false;
		immuneMagic		= false;

		deathParticleColor = Color(138, 209, 209);
	}

	sX = cX + spriteOffsetX;
	sY = cY + spriteOffsetY;

	lastAction = system_clock::now();
	lastTookDamage = system_clock::now();

	if (spawn) {
		gold = 0;

		spawning	= true;
		spawnLength = 1.0f;

		//visualEffects.push_back(VisualEffect());
		commands.push_back(Command(0, 0, false, "temp", 0, 0, 0.1f, 10, 1.0f));

		spawnStart = system_clock::now();
	}
}

void Wraith::update_wraith
(
	Player &player,
	std::vector<Tile> &tiles,
	std::vector<Command> &commands
)
{
	if (spawning) {
		spawn();
		return;
	}
	if (!active) {
		return;
	}

	explode(player, commands);
	move(player, tiles);
}

void Wraith::spawn()
{
	duration<float> elapsed_time = system_clock::now() - spawnStart;

	float secondsElapsed = elapsed_time.count();
	float progress = secondsElapsed / spawnLength;

	if (progress >= 0.95f) {
		spawning = false;

		lastAction = system_clock::now() - milliseconds(500);
	}
}

void Wraith::explode
(
	Player &player,
	std::vector<Command> &commands
)
{
	if (round(distance_real(Pos2D(player.cX, player.cY), Pos2D(cX, cY))) < player.hitboxRadius + hitboxRadius) {
		//player.damage(explosionDamage, D_MAGIC, NO_DIRECTION, commands);
		dead = true;
	}
}

void Wraith::move
(
	Player &player,
	std::vector<Tile> &tiles
)
{
	using namespace std::chrono;

	if (!inMA) {
		duration<float> elapsed_time = system_clock::now() - lastAction;
		if (elapsed_time < microseconds((int)(actionCooldown * 1000000))) return;

		std::string direction;

		if (player.gY == gY) {
			if (player.gX < gX) direction = "left";
			else				direction = "right";
		}
		else if (player.gX == gX) {
			if (player.gY < gY) direction = "up";
			else				direction = "down";
		}
		else {
			if (facing == "up") {
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

		currentAction = "move";
		spriteNumber  = "1";

		inMA		= true;
		MAdirection = direction;
		MAstart		= system_clock::now();
		sgX = gX;
		sgY = gY;

		c_MAlength = MAlength;

		facing = direction;
		if		(facing == "left")  spriteFacing = "left";
		else if (facing == "right") spriteFacing = "right";

		actionCooldown = 0.3f;
		lastAction = system_clock::now();

		// Set the tile the wraith is moving to as occupied to prevent other AI / player from entering it
		if		(direction == "up")    tiles[(gY - 1) * levelWidth + gX].occupied = true;
		else if (direction == "down")  tiles[(gY + 1) * levelWidth + gX].occupied = true;
		else if (direction == "left")  tiles[gY * levelWidth + gX - 1].occupied = true;
		else if (direction == "right") tiles[gY * levelWidth + gX + 1].occupied = true;
	}
	else if (inMA) {
		std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - MAstart;

		MAtimeElapsed = elapsed_seconds.count();
		MAprogress = MAtimeElapsed / c_MAlength;

		// Calculate distance moved while in the MA
		int movedBy;
		movedBy = cellSize - cellSize * (1.0f - MAprogress) * (1.0f - MAprogress);

		// If the wraith moved halfway to another tile, change its coordinates and update tile lighting info
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
		int height = 0;

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

void Wraith::end_MA()
{
	inMA			= false;
	changedPosition = false;
	currentAction	= "idle";
	spriteNumber	= "1";

	cX = gX * cellSize + cellSize / 2;
	cY = gY * cellSize + cellSize / 2;
}

void Wraith::damage
(
	int amount,
	std::string type,
	Direction direction,
	std::vector<Command> &commands
)
{
	AI::damage(amount, type, direction, commands, true);
}

void Wraith::damage
(
	DamageMap damageMap,
	std::vector<Command> &commands
)
{
	AI::damage(damageMap, commands, true);
}