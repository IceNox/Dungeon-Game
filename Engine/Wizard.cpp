#include "Wizard.h"

#include "GlobalData.h"

#include "Functions.h"

Wizard::Wizard
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
	spriteFacing  = "left";
	spriteNumber  = "1";

	cX = gX * cellSize + cellSize / 2;
	cY = gY * cellSize + cellSize / 2;

	floating = false;
	onGround = true;

	if (type == "small") {
		health			= 4;
		armor			= 0;
		maxHealth		= 4;
		healthbarHeight = 78;
		spriteOffsetX	= -36;
		spriteOffsetY	= -100;
		hitboxRadius	= 16;
		gold			= 25;

		boltDamage	   = 6;
		eruptionDamage = 5;

		resistancePhysical = 0.0f;
		resistanceFire	   = 0.0f;
		resistanceWater    = 0.0f;
		resistanceMagic    = 0.0f;

		immunePhysical	= false;
		immuneFire		= false;
		immuneWater		= false;
		immuneMagic		= true;

		deathParticleColor = Color(0, 90, 255);
	}

	sX = cX + spriteOffsetX;
	sY = cY + spriteOffsetY;

	revealed = false;

	lastAction	   = std::chrono::system_clock::now();
	lastTookDamage = std::chrono::system_clock::now();

	if (spawn) {
		gold = 0;

		spawning = true;
		spawnLength = 1.0f;

		//visualEffects.push_back(VisualEffect(0, 0, false, "temp", 0, 0, 0.1f, 10));
		commands.push_back(Command(0, 0, false, "temp", 0, 0, 0.1f, 10, 1.0f));

		spawnStart = system_clock::now();
	}
}

void Wizard::update_wizard
(
	Player &player,
	std::vector<Tile> &tiles,
	std::vector<DamageMap> &damageMap,
	std::vector<Command> &commands
)
{
	if (!active) return;

	attack(player, tiles, damageMap, commands);
	move(player, tiles);

	update_idle_animation();
}

void Wizard::attack
(
	Player &player,
	std::vector<Tile> &tiles,
	std::vector<DamageMap> &damageMap,
	std::vector<Command> &commands
)
{
	cast_bolt(player, tiles, commands);
	cast_slow(player, commands);
	cast_eruption(player, damageMap, commands);
}

void Wizard::cast_bolt
(
	Player &player,
	std::vector<Tile> &tiles,
	std::vector<Command> &commands
)
{
	using namespace std::chrono;

	if (!inMA && !inBA && !inEA && !inSA) {
		duration<float> elapsed_time;
		
		elapsed_time = system_clock::now() - lastAction;
		if (elapsed_time < microseconds((int)(actionCooldown * 1000000))) return;

		elapsed_time = system_clock::now() - BAlastCast;
		if (elapsed_time < microseconds((int)(boltCooldown	 * 1000000))) return;

		if (distance(Pos2D(player.gX, player.gY), Pos2D(gX, gY)) > 10) return;
		
		if (player.gX == gX) {
			if (player.gY < gY) {
				int x  = gX;
				int y1 = player.gY + 1;
				int y2 = gY - 1;

				for (int i = y1; i < y2; i++) {
					if (tiles[i * levelWidth + x].type == WALL && tiles[i * levelWidth + x].active) {
						return;
					}
				}

				inBA		= true;
				BAdirection = "up";
				BAstart		= system_clock::now();

				facing		  = "up";
				currentAction = "casting_bolt";
				spriteNumber  = "1";

				actionCooldown = 1.0f;
				lastAction = system_clock::now();
			}
			else if (player.gY > gY) {
				int x  = gX;
				int y1 = gY + 1;
				int y2 = player.gY - 1;

				for (int i = y1; i < y2; i++) {
					if (tiles[i * levelWidth + x].type == WALL && tiles[i * levelWidth + x].active) {
						return;
					}
				}

				inBA		= true;
				BAdirection = "down";
				BAstart		= system_clock::now();

				facing		  = "down";
				currentAction = "casting_bolt";
				spriteNumber  = "1";

				actionCooldown = 1.0f;
				lastAction = system_clock::now();
			}
		}
		else if (player.gY == gY) {
			if (player.gX < gX) {
				int x1 = player.gX + 1;
				int x2 = gX - 1;
				int y = gY;

				for (int i = x1; i < x2; i++) {
					if (tiles[y * levelWidth + i].type == WALL && tiles[y * levelWidth + i].active) {
						return;
					}
				}

				inBA		= true;
				BAdirection = "left";
				BAstart		= system_clock::now();

				facing		  = "left";
				spriteFacing  = "left";
				currentAction = "casting_bolt";
				spriteNumber  = "1";

				actionCooldown = 1.0f;
				lastAction = system_clock::now();
			}
			else if (player.gX > gX) {
				int x1 = gX + 1;
				int x2 = player.gX - 1;
				int y = gY;

				for (int i = x1; i < x2; i++) {
					if (tiles[y * levelWidth + i].type == WALL && tiles[y * levelWidth + i].active) {
						return;
					}
				}

				inBA		= true;
				BAdirection = "right";
				BAstart		= system_clock::now();

				facing		  = "right";
				spriteFacing  = "right";
				currentAction = "casting_bolt";
				spriteNumber  = "1";

				actionCooldown = 1.0f;
				lastAction = system_clock::now();
			}
		}
	}
	else if (inBA) {
		duration<float> elapsed_seconds = system_clock::now() - BAstart;

		BAtimeElapsed = elapsed_seconds.count();
		BAprogress	  = BAtimeElapsed / BAlength;

		if (BAprogress < 0.3f) {
			spriteNumber = "1";
		}
		else if (BAprogress < 1.0f) {
			if (spriteNumber == "1") {
				std::string spriteName = "wizard_bolt_" + BAdirection;

				if		(BAdirection == "up")    commands.push_back(Command(gX, gY, cellSize / 2, cellSize / 2 - 24, boltDamage, 0, 12, -12, -12 - 32, true, false, 0.0f, -500.0f, 10000, 10000, true, spriteName, 24, 48, 2, 0.1f));
				else if (BAdirection == "down")  commands.push_back(Command(gX, gY, cellSize / 2, cellSize / 2 + 24, boltDamage, 0, 12, -12, -36 - 32, true, false, 0.0f,  500.0f, 10000, 10000, true, spriteName, 24, 48, 2, 0.1f));
				else if (BAdirection == "left")  commands.push_back(Command(gX, gY, cellSize / 2 - 24, cellSize / 2, boltDamage, 0, 12, -12, -12 - 32, true, false, -500.0f, 0.0f, 10000, 10000, true, spriteName, 48, 24, 2, 0.1f));
				else if (BAdirection == "right") commands.push_back(Command(gX, gY, cellSize / 2 + 24, cellSize / 2, boltDamage, 0, 12, -36, -12 - 32, true, false,  500.0f, 0.0f, 10000, 10000, true, spriteName, 48, 24, 2, 0.1f));
			}

			spriteNumber = "2";
		}
		else if (BAprogress >= 1.0f) {
			inBA		  = false;
			currentAction = "idle";
			spriteNumber  = "1";
			lastAction	  = system_clock::now();
			BAlastCast	  = system_clock::now();
		}
	}
}

void Wizard::cast_slow
(
	Player &player,
	std::vector<Command> &commands
)
{
	using namespace std::chrono;

	if (!inMA && !inBA && !inEA && !inSA) {
		duration<float> elapsed_time;

		elapsed_time = system_clock::now() - lastAction;
		if (elapsed_time < microseconds((int)(actionCooldown * 1000000))) return;

		elapsed_time = system_clock::now() - SAlastCast;
		if (elapsed_time < microseconds((int)(slowCooldown * 1000000))) return;

		if (distance(Pos2D(player.gX, player.gY), Pos2D(gX, gY)) > 5) return;

		inSA	= true;
		SAstart = system_clock::now();

		currentAction = "casting_slow";
		spriteNumber  = "1";

		actionCooldown = 1.0f;
		lastAction = system_clock::now();
	}
	else if (inSA) {
		duration<float> elapsed_seconds = system_clock::now() - SAstart;

		SAtimeElapsed = elapsed_seconds.count();
		SAprogress	  = SAtimeElapsed / SAlength;

		if (SAprogress >= 1.0f) {
			inSA	      = false;
			currentAction = "idle";
			spriteNumber  = "1";
			//lastAction	  = system_clock::now();
			SAlastCast    = system_clock::now();

			player.statusEffects.slow(0.5f, 2.0f);
			//screenAnimations.start_slow();
		}
	}
}

void Wizard::cast_eruption
(
	Player &player,
	std::vector<DamageMap>& damageMap,
	std::vector<Command> &commands
)
{
	using namespace std::chrono;

	if (!inMA && !inBA && !inEA && !inSA) {
		duration<float> elapsed_time;

		elapsed_time = system_clock::now() - lastAction;
		if (elapsed_time < microseconds((int)(actionCooldown * 1000000))) return;

		elapsed_time = system_clock::now() - EAlastCast;
		if (elapsed_time < microseconds((int)(boltCooldown * 1000000))) return;

		if (distance(Pos2D(player.gX, player.gY), Pos2D(gX, gY)) > 5) return;

		inEA	= true;
		EAstart = system_clock::now();

		actionCooldown = 1.0f;
		lastAction = system_clock::now();

		currentAction = "casting_eruption";
		spriteNumber  = "1";

		egX = player.gX;
		egY = player.gY;

		//visualEffects.push_back(VisualEffect(egX * cellSize, egY * cellSize, true, "eruption_windup_effect", 96, 96, 0.0675f, 8));
		commands.push_back(Command(egX * cellSize, egY * cellSize, true, "eruption_windup_effect", 96, 96, 0.0675f, 8, 1.0f));
	}
	else if (inEA) {
		duration<float> elapsed_seconds = system_clock::now() - EAstart;

		EAtimeElapsed = elapsed_seconds.count();
		EAprogress    = EAtimeElapsed / EAlength;

		if (EAprogress >= 1.0f) {
			inEA = false;
			currentAction = "idle";
			spriteNumber  = "1";
			//lastAction	  = system_clock::now();
			EAlastCast	  = system_clock::now();

			damageMap[egY * levelWidth + egX].active = true;
			damageMap[egY * levelWidth + egX].playerDamageMagic = eruptionDamage;

			//visualEffects.push_back(VisualEffect(egX * cellSize - 8, egY * cellSize - 96, false, "eruption_effect", 112, 160, 0.03f, 9));
			commands.push_back(Command(egX * cellSize - 8, egY * cellSize - 96, false, "eruption_effect", 112, 160, 0.03f, 9, 1.0f));
		}
	}
}

void Wizard::move
(
	Player &player,
	std::vector<Tile> &tiles
)
{
	using namespace std::chrono;

	if (!inMA && !inBA && !inEA && !inSA) {
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

		if (direction == "up") {
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
				if		(player.gY < gY) direction = "up";
				else if (player.gY > gY) direction = "down";
				else					 direction = "left";
			}
		}
		else if (direction == "right") {
			if (tiles[gY * levelWidth + gX + 1].occupied) {
				if		(player.gY < gY) direction = "up";
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

		actionCooldown = 1.0f;
		lastAction = system_clock::now();

		// Set the tile the wizard is moving to as occupied to prevent other AI / player from entering it
		if		(direction == "up")	   tiles[(gY - 1) * levelWidth + gX].occupied = true;
		else if (direction == "down")  tiles[(gY + 1) * levelWidth + gX].occupied = true;
		else if (direction == "left")  tiles[gY * levelWidth + gX - 1].occupied = true;
		else if (direction == "right") tiles[gY * levelWidth + gX + 1].occupied = true;
	}
	else if (inMA) {
		duration<float> elapsed_seconds = system_clock::now() - MAstart;

		MAtimeElapsed = elapsed_seconds.count();
		MAprogress	  = MAtimeElapsed / c_MAlength;

		// Calculate distance moved while in the MA
		int movedBy;
		movedBy = cellSize - cellSize * (1.0f - MAprogress) * (1.0f - MAprogress);

		// If the wizard moved halfway to another tile, change his coordinates
		if (MAprogress > 0.3f && changedPosition == false) {
			changedPosition = true;

			if (MAdirection == "up") {
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
		if (MAprogress < 0.5f) spriteNumber = "1";
		else				   spriteNumber = "2";

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

		// Otherwise, update wizard position
		else {
			if (MAdirection == "up") {
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

		// Update wizard sprite position
		sX = cX + spriteOffsetX;
		sY = cY + spriteOffsetY;

		if (inMA) sY -= height;
	}
}

void Wizard::end_MA()
{
	inMA			= false;
	onGround		= true;
	changedPosition = false;
	currentAction	= "idle";
	spriteNumber	= "1";

	cX = gX * cellSize + cellSize / 2;
	cY = gY * cellSize + cellSize / 2;
}

void Wizard::update_idle_animation()
{
	if (!inMA && !inBA && !inSA && !inEA) {
		duration<float> elapsed_time = system_clock::now() - lastAction;
		if ((int)(elapsed_time.count() / (actionCooldown / 2)) % 2 == 0) {
			spriteNumber = "1";
		}
		else {
			spriteNumber = "2";
		}
	}
}

void Wizard::damage
(
	int amount,
	std::string type,
	Direction direction,
	std::vector<Command> &commands
)
{
	AI::damage(amount, type, direction, commands, true);
}

void Wizard::damage
(
	DamageMap damageMap,
	std::vector<Command> &commands
)
{
	AI::damage(damageMap, commands, true);
}