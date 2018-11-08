#pragma once

#include "LevelData.h"

#include <string>
#include <vector>

class Level;

struct PlayerInventory
{
	std::string playerItems[5];

	PlayerInventory(std::string items[])
	{
		for (int i = 0; i < 5; i++) {
			playerItems[i] = items[i];
		}
	};
};

class SaveData
{
public:
	std::string saveName;

	// General data
	int currentStage;
	int currentLevel;

	// Player data
	int playerCount;

	std::vector<Pos2D> playerGridPos;

	std::vector<int> playerHealth;
	std::vector<int> playerArmor;
	std::vector<int> playerGold;

	std::vector<PlayerInventory> playerInventory;

	// Level data
	std::vector<LevelData> levelData;

	// Functions
	void read_from_file(std::string SaveName);
	void write_to_file(Level *level);
};