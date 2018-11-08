#include "Paladin.h"

#include "GlobalData.h"
#include "Constants.h"

#include "Functions.h"

#include "Sprite.h"

Paladin::Paladin
(
	int gX,
	int gY,
	std::string type,
	std::vector<Command> &commands,
	bool spawn
)
{
	using namespace std::chrono;

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
		spriteOffsetX	= -56;
		spriteOffsetY	= -96;
		hitboxRadius	= 32;
		gold			= 30;

		hitDamage	 = 3;
		chargeDamage = 6;
		blockDamage  = 5;

		resistancePhysical = 0.0f;
		resistanceFire	   = 0.0f;
		resistanceWater    = 0.0f;
		resistanceMagic    = 0.0f;

		immunePhysical	= false;
		immuneFire		= false;
		immuneWater		= false;
		immuneMagic		= false;

		deathParticleColor = Color(255, 255, 140);
	}

	sX = cX + spriteOffsetX;
	sY = cY + spriteOffsetY;

	revealed = false;

	lastAction	   = system_clock::now();
	lastTookDamage = system_clock::now();

	lastBlock  = system_clock::now() - seconds(10);
	lastCharge = system_clock::now() - seconds(10);

	if (spawn) {
		gold = 0;

		spawning = true;
		spawnLength = 1.0f;

		//visualEffects.push_back(VisualEffect(0, 0, false, "temp", 0, 0, 0.1f, 10));
		commands.push_back(Command(0, 0, false, "temp", 0, 0, 0.1f, 10, 1.0f));

		spawnStart = system_clock::now();
	}
}

void Paladin::update_paladin
(
	Player &player,
	std::vector<Tile> &tiles,
	std::vector<DamageMap> &damageMap,
	std::vector<Command> &commands
)
{
	if (!active) return;

	block(player, damageMap, commands);
	charge(player, tiles, damageMap, commands);
	attack(player, damageMap, commands);
	move(player, tiles);

	update_idle_animation();
}

void Paladin::block
(
	Player &player,
	std::vector<DamageMap> &damageMap,
	std::vector<Command> &commands
)
{
	if (inMA || inAA || inCA) return;

	if (blockedDamage) {
		inBA = false;

		currentAction = "idle";
		spriteNumber  = "2";

		blockedDamage = false;

		if		(BAdirection == "left") {
			if (player.gX == gX - 1 && player.gY == gY) {
				//player.damage(blockDamage, "physical", "left", particles, screenAnimations);
				damageMap[player.gY * levelWidth + player.gX].playerDamagePhysical = blockDamage;
				damageMap[player.gY * levelWidth + player.gX].direction = LEFT;
				damageMap[player.gY * levelWidth + player.gX].active = true;
				player.statusEffects.stun(1.5f);
			}
		}
		else if (BAdirection == "right") {
			if (player.gX == gX + 1 && player.gY == gY) {
				//player.damage(blockDamage, "physical", "right", particles, screenAnimations);
				damageMap[player.gY * levelWidth + player.gX].playerDamagePhysical = blockDamage;
				damageMap[player.gY * levelWidth + player.gX].direction = RIGHT;
				damageMap[player.gY * levelWidth + player.gX].active = true;
				player.statusEffects.stun(1.5f);
			}
		}

		lastBlock = system_clock::now();
	}

	duration<float> elapsed_time;

	elapsed_time = system_clock::now() - lastAction;
	if (elapsed_time < microseconds((int)((actionCooldown - 1.0f) * 1000000))) return;

	elapsed_time = system_clock::now() - lastBlock;
	if (elapsed_time < microseconds((int)(BAcooldown * 1000000))) return;

	if (player.statusEffects.stunned) return;

	if		(player.gX == gX - 1 && player.gY == gY) {
		inBA		= true;
		BAdirection = "left";
		facing		= "left";

		currentAction = "blocking";
		spriteFacing  = "left";
		spriteNumber  = "1";
	}
	else if (player.gX == gX + 1 && player.gY == gY) {
		inBA		= true;
		BAdirection = "right";
		facing		= "right";

		currentAction = "blocking";
		spriteFacing  = "right";
		spriteNumber  = "1";
	}
	else if (inBA) {
		inBA = false;

		currentAction = "idle";
		spriteNumber  = "2";
	}
}

void Paladin::charge
(
	Player &player,
	std::vector<Tile> &tiles,
	std::vector<DamageMap> &damageMap,
	std::vector<Command> &commands
)
{
	if (!inMA && !inAA && !inBA && !inCA) {
		duration<float> elapsed_time;

		elapsed_time = system_clock::now() - lastAction;
		if (elapsed_time < microseconds((int)((actionCooldown - 0.25f) * 1000000))) return;

		elapsed_time = system_clock::now() - lastCharge;
		if (elapsed_time < microseconds((int)(CAcooldown * 1000000))) return;

		if (distance(Pos2D(player.gX, player.gY), Pos2D(gX, gY)) > 4) return;

		if (player.gY == gY) {
			if (player.gX < gX) {
				int x1 = player.gX + 1;
				int x2 = gX - 1;
				int y  = gY;

				for (int i = x1; i < x2; i++) {
					if (tiles[y * levelWidth + i].occupied) {
						return;
					}
				}

				inCA		= true;
				CAdirection = "left";
				CAstart		= system_clock::now();

				facing = "left";

				spriteFacing  = "left";
				currentAction = "charging";
				spriteNumber  = "1";

				actionCooldown = 1.0f;
				lastAction = system_clock::now();
			}
			else if (player.gX > gX) {
				int x1 = gX + 1;
				int x2 = player.gX - 1;
				int y  = gY;

				for (int i = x1; i < x2; i++) {
					if (tiles[y * levelWidth + i].occupied) {
						return;
					}
				}

				inCA		= true;
				CAdirection = "right";
				CAstart		= system_clock::now();

				facing = "right";

				spriteFacing  = "right";
				currentAction = "charging";
				spriteNumber  = "1";

				actionCooldown = 1.0f;
				lastAction = system_clock::now();
			}
		}
	}
	else if (!inMA && !inAA && !inBA && inCA) {
		duration<float> elapsed_seconds = system_clock::now() - CAstart;

		if (elapsed_seconds.count() >= CAlength) {
			bool hitPlayer = false;

			if (CAdirection == "left") {
				int ngX = gX;

				for (int i = 0; i < 4; i++) {
					ngX--;
					int index = gY * levelWidth + ngX;

					if (tiles[index].occupied) {
						// Case 1 - anything but player
						if (!player.inMA && player.gY * levelWidth + player.gX != index) {
							ngX++;
							break;
						}

						// Case 2 - non-moving player
						if (!player.inMA && player.gY * levelWidth + player.gX == index) {
							ngX++;

							//player.damage(chargeDamage, "physical", "left", particles, screenAnimations);
							damageMap[player.gY * levelWidth + player.gX].playerDamagePhysical = chargeDamage;
							damageMap[player.gY * levelWidth + player.gX].direction = LEFT;
							damageMap[player.gY * levelWidth + player.gX].active = true;
							player.statusEffects.stun(2.0f);
							hitPlayer = true;

							break;
						}

						// Case 3 - player moving out of the tile
						if (player.inMA && player.gY * levelWidth + player.gX == index) {
							ngX++;

							player.end_MA();
							//player.damage(chargeDamage, "physical", "left", particles, screenAnimations);
							damageMap[player.gY * levelWidth + player.gX].playerDamagePhysical = chargeDamage;
							damageMap[player.gY * levelWidth + player.gX].direction = LEFT;
							damageMap[player.gY * levelWidth + player.gX].active = true;
							player.statusEffects.stun(2.0f);
							hitPlayer = true;

							break;
						}

						// Case 4 - player moving into the tile
						if (player.inMA && player.gY * levelWidth + player.gX != index) {
							int pindex;

							if		(player.MAdirection == UP)		pindex = (player.gY - 1) * levelWidth + player.gX;
							else if (player.MAdirection == DOWN)	pindex = (player.gY + 1) * levelWidth + player.gX;
							else if (player.MAdirection == LEFT)	pindex = player.gY * levelWidth + player.gX - 1;
							else if (player.MAdirection == RIGHT)	pindex = player.gY * levelWidth + player.gX + 1;
							
							if (pindex == index) {
								player.end_MA();
								//player.damage(chargeDamage, "physical", "left", particles, screenAnimations);
								damageMap[player.gY * levelWidth + player.gX].playerDamagePhysical = chargeDamage;
								damageMap[player.gY * levelWidth + player.gX].direction = LEFT;
								damageMap[player.gY * levelWidth + player.gX].active = true;
								player.statusEffects.stun(2.0f);
								hitPlayer = true;
							}

							break;
						}
					}
				}

				int moveDistance = gX - ngX;

				gX = ngX;

				cX = gX * cellSize + cellSize / 2;
				cY = gY * cellSize + cellSize / 2;

				sX = cX + spriteOffsetX;
				sY = cY + spriteOffsetY;

				inCA	      = false;
				currentAction = "idle";
				spriteNumber  = "2";

				lastCharge = system_clock::now();

				// Create Particle trail
				int spriteW = (moveDistance - 1) * cellSize + 34;
				int spriteH = 4;

				if (spriteW > 0) {
					Sprite s = create_rectangle(spriteW, spriteH, Colors::White);

					//particles.push_back(Particle(cX + 64, cY - 30, s, 0, 0, 0, 0, 0.15f, 0.15f));
					commands.push_back(Command(cX + 64, cY - 30, s, 0, 0, 0, 0, 0.15f, 0.15f));
				}
			}
			else if (CAdirection == "right") {
				int ngX = gX;

				for (int i = 0; i < 4; i++) {
					ngX++;
					int index = gY * levelWidth + ngX;

					if (tiles[index].occupied) {
						// Case 1 - anything but player
						if (!player.inMA && player.gY * levelWidth + player.gX != index) {
							ngX--;
							break;
						}

						// Case 2 - non-moving player
						if (!player.inMA && player.gY * levelWidth + player.gX == index) {
							ngX--;

							//player.damage(chargeDamage, "physical", "right", particles, screenAnimations);
							damageMap[player.gY * levelWidth + player.gX].playerDamagePhysical = chargeDamage;
							damageMap[player.gY * levelWidth + player.gX].direction = RIGHT;
							damageMap[player.gY * levelWidth + player.gX].active = true;
							player.statusEffects.stun(2.0f);
							hitPlayer = true;

							break;
						}

						// Case 3 - player moving out of the tile
						if (player.inMA && player.gY * levelWidth + player.gX == index) {
							ngX--;

							player.end_MA();
							//player.damage(chargeDamage, "physical", "right", particles, screenAnimations);
							damageMap[player.gY * levelWidth + player.gX].playerDamagePhysical = chargeDamage;
							damageMap[player.gY * levelWidth + player.gX].direction = RIGHT;
							damageMap[player.gY * levelWidth + player.gX].active = true;
							player.statusEffects.stun(2.0f);
							hitPlayer = true;

							break;
						}

						// Case 4 - player moving into the tile
						if (player.inMA && player.gY * levelWidth + player.gX != index) {
							int pindex;

							if		(player.MAdirection == UP)		pindex = (player.gY - 1) * levelWidth + player.gX;
							else if (player.MAdirection == DOWN)	pindex = (player.gY + 1) * levelWidth + player.gX;
							else if (player.MAdirection == LEFT)	pindex = player.gY * levelWidth + player.gX - 1;
							else if (player.MAdirection == RIGHT)	pindex = player.gY * levelWidth + player.gX + 1;

							if (pindex == index) {
								player.end_MA();
								//player.damage(chargeDamage, "physical", "right", particles, screenAnimations);
								damageMap[player.gY * levelWidth + player.gX].playerDamagePhysical = chargeDamage;
								damageMap[player.gY * levelWidth + player.gX].direction = RIGHT;
								damageMap[player.gY * levelWidth + player.gX].active = true;
								player.statusEffects.stun(2.0f);
								hitPlayer = true;
							}

							break;
						}
					}
				}

				int moveDistance = ngX - gX;

				gX = ngX;

				cX = gX * cellSize + cellSize / 2;
				cY = gY * cellSize + cellSize / 2;

				sX = cX + spriteOffsetX;
				sY = cY + spriteOffsetY;

				inCA		  = false;
				currentAction = "idle";
				spriteNumber  = "2";

				lastCharge = system_clock::now();

				// Create Particle trail
				int spriteW = (moveDistance - 1) * cellSize + 34;
				int spriteH = 4;

				if (spriteW > 0) {
					Sprite s = create_rectangle(spriteW, spriteH, Colors::White);

					//particles.push_back(Particle(cX - spriteW - 64, cY - 30, s, 0, 0, 0, 0, 0.15f, 0.15f));
					commands.push_back(Command(cX - spriteW - 64, cY - 30, s, 0, 0, 0, 0, 0.15f, 0.15f));
				}
			}

			if (!hitPlayer) {
				// Make paladin tired or some shit
				actionCooldown = 3.0f;
			}
		}
	}
}

void Paladin::attack
(
	Player &player,
	std::vector<DamageMap> &damageMap,
	std::vector<Command> &commands
)
{
	using namespace std::chrono;

	if (!inMA && !inAA && !inBA && !inCA) {
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

			actionCooldown = 1.0f;
			lastAction = system_clock::now();
		}
	}
	else if (!inMA && !inBA && !inCA && inAA) {
		// Update time
		duration<float> elapsed_seconds = system_clock::now() - AAstart;

		AAtimeElapsed = elapsed_seconds.count();
		AAprogress	  = AAtimeElapsed / AAlength;

		// End AA if nescessary
		if (AAprogress >= 1.0f) {
			inAA		  = false;
			currentAction = "idle";
			spriteNumber  = "1";

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

			if (AAdirection == "up") {
				soX = 56;
				soY = -48;

				sprX = 8;
			}
			else if (AAdirection == "down") {
				soX = 48;
				soY = 48;

				sprX = 8;
			}
			else if (AAdirection == "left") {
				soX = -32;
				soY = 12;

				sprX = 24;
			}
			else if (AAdirection == "right") {
				soX = 120;
				soY = 12;

				sprX = 24;
			}

			//visualEffects.push_back(VisualEffect(sX + soX, sY + soY, false, "attack_chop_" + AAdirection + "_effect", sprX, sprY, 0.02f, 7));
			commands.push_back(Command(sX + soX, sY + soY, false, "attack_chop_" + AAdirection + "_effect", sprX, sprY, 0.02f, 7, 1.0f));
		}
	}
}

void Paladin::move
(
	Player &player,
	std::vector<Tile> &tiles
)
{
	using namespace std::chrono;

	if (!inMA && !inAA && !inBA && !inCA) {
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

		// Set the tile the paladin is moving to as occupied to prevent other AI / player from entering it
		if		(direction == "up")	   tiles[(gY - 1) * levelWidth + gX].occupied = true;
		else if (direction == "down")  tiles[(gY + 1) * levelWidth + gX].occupied = true;
		else if (direction == "left")  tiles[gY * levelWidth + gX - 1].occupied = true;
		else if (direction == "right") tiles[gY * levelWidth + gX + 1].occupied = true;
	}
	else if (inMA && !inAA && !inBA && !inCA) {
		duration<float> elapsed_seconds = system_clock::now() - MAstart;

		MAtimeElapsed = elapsed_seconds.count();
		MAprogress = MAtimeElapsed / c_MAlength;

		// Calculate distance moved while in the MA
		int movedBy;
		movedBy = cellSize - cellSize * (1.0f - MAprogress) * (1.0f - MAprogress);

		// If the paladin moved halfway to another tile, change his coordinates
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

		// Otherwise, update paladin position
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

		// Update paladin sprite position
		sX = cX + spriteOffsetX;
		sY = cY + spriteOffsetY;

		if (inMA) sY -= height;
	}
}

void Paladin::end_MA()
{
	inMA			= false;
	onGround		= true;
	changedPosition = false;
	currentAction	= "idle";
	spriteNumber	= "1";

	cX = gX * cellSize + cellSize / 2;
	cY = gY * cellSize + cellSize / 2;
}

void Paladin::update_idle_animation()
{
	if (!inMA && !inAA && !inBA && !inCA) {
		duration<float> elapsed_time = system_clock::now() - lastAction;
		if ((int)(elapsed_time.count() / (actionCooldown / 2)) % 2 == 0) {
			spriteNumber = "1";
		}
		else {
			spriteNumber = "2";
		}
	}
}

void Paladin::damage
(
	int amount,
	std::string type,
	Direction direction,
	std::vector<Command> &commands
)
{
	DamageMap dm;
	dm.direction = direction;

	if		(type == "physical") dm.playerDamagePhysical += amount;
	else if (type == "fire")	 dm.playerDamageFire	 += amount;
	else if (type == "water")	 dm.playerDamageWater	 += amount;
	else if (type == "magic")	 dm.playerDamageMagic	 += amount;

	damage(dm, commands);
}

void Paladin::damage
(
	DamageMap damageMap,
	std::vector<Command> &commands
)
{
	if (inBA) {
		if (BAdirection == "left" && damageMap.direction == RIGHT) {
			blockedDamage = true;
			return;
		}
		if (BAdirection == "right" && damageMap.direction == LEFT) {
			blockedDamage = true;
			return;
		}
	}

	AI::damage(damageMap, commands, true);
}