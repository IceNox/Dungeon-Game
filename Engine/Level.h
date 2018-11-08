#pragma once

#include "Constants.h"

#include "GameTime.h"

#include "GameMessage.h"
#include "LevelMessage.h"
#include "Command.h"

#include "Player.h"

#include "Tile.h"
#include "Wire.h"
#include "LevelStateData.h"

#include "Portal.h"

#include "LevelObject.h"

#include "Crate.h"
#include "PressurePlate.h"
#include "Lamp.h"
#include "Door.h"
#include "Sign.h"

#include "Shooter.h"
#include "Barbarian.h"
#include "Wizard.h"
#include "Paladin.h"
#include "Zombie.h"
#include "Lich.h"
#include "Wraith.h"

#include "Entity.h"

#include "HealthMap.h"

#include "Minimap.h"

#include "ScreenAnimations.h"
#include "Animation.h"
#include "Particle.h"

#include "Keys.h"
#include "KeyBindings.h"

#include "LevelData.h"
#include "SaveData.h"
#include "UserData.h"

#include <vector>

using namespace std;

class Level
{
public:
	// General
	LevelType levelType;

	int currentStage;
	int currentLevel;

	int sl_index; // Index that shows which array element the current stage and level refer to

	int width;
	int height;

	TimePoint levelStartTime;
	TimePoint levelEndTime;
	std::string levelTime;

	bool paused = false;
	time_point<system_clock> pauseStartTime;

	// Elements
	std::vector<LevelMessage> levelMessages;
	std::vector<Command> commands;

	std::vector<Player> players;

	bool reachedExit;
	bool playerDied;

	TileVariant baseFloorVariant;

	int tileCount;
	int wireCount;
	std::vector<Tile> tiles;
	std::vector<Wire> wires;

	LevelStateData levelStateData;

	int sObjectCount;
	int dObjectCount;
	std::vector<LevelObject*> levelObjects;

	// UNNESCESSARY
	int crateCount;
	int pressurePlateCount;
	int lampCount;
	int doorCount;
	int signCount;
	std::vector<Crate> crates;
	std::vector<PressurePlate> pressurePlates;
	std::vector<Lamp> lamps;
	std::vector<Door> doors;
	std::vector<Sign> signs;

	int shooterCount;
	int barbarianCount;
	int wizardCount;
	int paladinCount;
	int zombieCount;
	int lichCount;
	int wraithCount;
	std::vector<Shooter> shooters;
	std::vector<Barbarian> barbarians;
	std::vector<Wizard> wizards;
	std::vector<Paladin> paladins;
	std::vector<Zombie> zombies;
	std::vector<Lich> liches;
	std::vector<Wraith> wraiths;
	// UNNESCESSARY


	std::vector<Entity> entities;

	std::vector<Animation> animations;
	std::vector<Particle> particles;

	std::vector<DamageMap> damageMap;
	std::vector<HealMap> healMap;
	std::vector<HealthMap> healthMap;

	Minimap minimap;

	int portalCount;
	std::vector<Portal> portals;
	
	// Current save
	SaveData saveData;

	// Gameplay
	Pos2D visionCenterPos;

/// Various animation variables
	// Death screen
	bool inDeathScreen = false;
	TimePoint deathScreenStart;
	bool startedDeathEffect = false;

	// Level end screen
	bool inEndScreen = false;
	TimePoint endScreenStart;
	bool startedEndEffect = false;

	// End/Death screen fade animation
	const int EDFAlength = 250;
	bool inEDFA;
	float EDFAprogress;
	TimePoint EDFAstart;
	std::string newMode;

	// Level start fade animation
	const int FAlength = 250;
	bool inFA;
	float FAprogress;
	TimePoint FAstart;

/// Functions
	Level();
	void load_game(LevelType lType, std::string lName);

	void load_level(std::string levelName) {};

	void load_save_file(std::string saveName);
	void save_save_file() {};

	void update_level(std::vector<GameMessage*> &msg, ScreenAnimations &screenAnimations, Keys &keys, UserData &userData);
	void set_game_state_data();
	void handle_messages();

	void collide_entities(ScreenAnimations &screenAnimations);
	void deal_damage(ScreenAnimations &screenAnimations);

	void remove_destroyed_objects();
	void remove_finished_animations();
	void remove_finished_particles();

	void merge_gold_on_ground();

	void update_minimap(Keys &keys, UserData &userData);
	void update_tile_info();
	void update_wires();
	void update_lighting();
	void update_visibility();

	void finish_level();
};