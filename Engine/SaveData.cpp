#include "SaveData.h"

#include "json.hpp"

#include "Functions.h"
#include "GlobalData.h"

#include "Level.h"

#include <fstream>

using json = nlohmann::json;

void SaveData::read_from_file(std::string fileName)
{
	// Clear data
	playerGridPos.clear();
	playerHealth.clear();
	playerArmor.clear();
	playerGold.clear();
	levelData.clear();

	// Open file from directory and parse it into json state
	std::string path = "Content/Saves/" + fileName;
	std::ifstream in(path);

	// Gat raw data
	in.seekg(0, std::ios::end);
	int length = in.tellg();
	in.seekg(0, std::ios::beg);
	char *buffer = new char[length];
	in.read(buffer, length);
	in.close();

	// Parse it into a json state and release buffer memory
	json save = json::parse(buffer);
	buffer = nullptr;
	delete buffer;

	// Read main info
	saveName = save["saveName"].get<std::string>();

	currentStage = save["currentStage"];
	currentLevel = save["currentLevel"];

	// Read player info
	playerCount = save["players"].size();

	for (int i = 0; i < playerCount; i++) {
		// Position
		playerGridPos.push_back(
			Pos2D(
				save["players"][i]["x"],
				save["players"][i]["y"]
			)
		);

		// Stats
		playerHealth.push_back(save["players"][i]["health"]);
		playerArmor.push_back(save["players"][i]["armor"]);
		playerGold.push_back(save["players"][i]["gold"]);
	}

	// Read levels
	for (int i = 0; i < 2; i++) {
		levelData.push_back(LevelData());

		levelData[i].levelName = ".";

		// Get level dimensions
		levelData[i].width	= save["levels"][i]["width"];
		levelData[i].height = save["levels"][i]["height"];

		// Get player spawn locations
		levelData[i].playerStartPos = Pos2D
		(
			save["levels"][i]["entryPos"]["x"],
			save["levels"][i]["entryPos"]["y"]
		);

		// Get portals
		levelData[i].portalCount = save["levels"][i]["portals"].size();

		for (int j = 0; j < levelData[i].portalCount; j++) {
			levelData[i].portalPos.push_back(Pos2D(
				save["levels"][i]["portals"][j]["entryPos"]["x"],
				save["levels"][i]["portals"][j]["entryPos"]["y"]
			));
			levelData[i].portalExitPos.push_back(Pos2D(
				save["levels"][i]["portals"][j]["exitPos"]["x"],
				save["levels"][i]["portals"][j]["exitPos"]["y"]
			));
			levelData[i].portalNewLevel.push_back(
				save["levels"][i]["portals"][j]["exitLevel"]
			);
		}

		// Get walls
		levelData[i].wallCount = save["levels"][i]["walls"].size();

		for (int j = 0; j < levelData[i].wallCount; j++) {
			levelData[i].wallGridPos.push_back(Pos2D(
				save["levels"][i]["walls"][j]["pos"]["x"],
				save["levels"][i]["walls"][j]["pos"]["y"]
			));
			levelData[i].wallVariant.push_back(static_cast<TileVariant>(save["levels"][i]["walls"][j]["variant"]));

			levelData[i].wallActivatable.push_back(save["levels"][i]["walls"][j]["activatable"]);
			levelData[i].wallToggleable.push_back(save["levels"][i]["walls"][j]["toggleable"]);
			levelData[i].wallActive.push_back(save["levels"][i]["walls"][j]["active"]);
		}

		// Get floor
		levelData[i].baseFloorVariant = static_cast<TileVariant>(save["levels"][i]["baseFloorVariant"]);

		levelData[i].floorCount = save["levels"][i]["floors"].size();

		for (int j = 0; j < levelData[i].floorCount; j++) {
			levelData[i].floorGridPos.push_back(Pos2D(
				save["levels"][i]["floors"][j]["pos"]["x"],
				save["levels"][i]["floors"][j]["pos"]["y"]
			));
			levelData[i].floorVariant.push_back(static_cast<TileVariant>(save["levels"][i]["floors"][j]["variant"]));
		}

		// Get static objects
		levelData[i].sObjectCount = save["levels"][i]["sObjects"].size();

		for (int j = 0; j < levelData[i].sObjectCount; j++) {
			levelData[i].sObjectID.push_back(save["levels"][i]["sObjects"][j]["id"]);
			levelData[i].sObjectPos.push_back(Pos2D(
				save["levels"][i]["sObjects"][j]["pos"]["x"],
				save["levels"][i]["sObjects"][j]["pos"]["y"]
			));
		}
	}

	/*
	// Read main info
	getline(in, saveName);

	in >> currentStage >> currentLevel;

	// Read player info
	in >> playerCount;
	for (int i = 0; i < playerCount; i++) {
		// Position
		int px, py;
		in >> px >> py;
		playerGridPos.push_back(Pos2D(px, py));

		// Stats
		int hp, ar, gd;
		in >> hp >> ar >> gd;
		playerHealth.push_back(hp);
		playerArmor.push_back(ar);
		playerGold.push_back(gd);

		// Inventory
		std::string itemNames[5];
		for (int j = 0; j < 5; j++) {
			in >> itemNames[i];
		}

		playerInventory.push_back(PlayerInventory(itemNames));
	}

	// Read level data
	for (int i = 0; i < 12; i++) {
		levelData.push_back(LevelData());

		levelData[i].levelName = ".";
		int gx, gy;

		// Get level dimensions
		in >> levelData[i].width;
		in >> levelData[i].height;

		// Get player spawn locations
		in >> gx >> gy;
		levelData[i].playerStartPos[0] = Pos2D(gx, gy);

		in >> gx >> gy;
		levelData[i].playerStartPos[1] = Pos2D(gx, gy);

		// Get portals
		in >> levelData[i].portalCount;

		for (int j = 0; j < levelData[i].portalCount; j++) {
			in >> gx >> gy;
			levelData[i].portalPos.push_back(Pos2D(gx, gy));
			
			in >> gx >> gy;
			levelData[i].portalExitPos.push_back(Pos2D(gx, gy));
			
			int pnl; in >> pnl;
			levelData[i].portalNewLevel.push_back(pnl);
		}

		// Get walls
		in >> levelData[i].wallCount;

		for (int j = 0; j < levelData[i].wallCount; j++) {
			in >> gx >> gy;
			levelData[i].wallGridPos.push_back(Pos2D(gx, gy));

			int wallVariant; in >> wallVariant;
			levelData[i].wallVariant.push_back(static_cast<TileVariant>(wallVariant));

			bool ws;
			
			in >> ws;
			levelData[i].wallActivatable.push_back(ws);

			in >> ws;
			levelData[i].wallToggleable.push_back(ws);

			in >> ws;
			levelData[i].wallActive.push_back(ws);
		}

		// Get floor
		int baseFloorVariant; in >> baseFloorVariant;
		levelData[i].baseFloorVariant = static_cast<TileVariant>(baseFloorVariant);

		in >> levelData[i].floorCount;

		for (int j = 0; j < levelData[i].floorCount; j++) {
			in >> gx >> gy;
			levelData[i].floorGridPos.push_back(Pos2D(gx, gy));

			int floorVariant; in >> floorVariant;
			levelData[i].floorVariant.push_back(static_cast<TileVariant>(floorVariant));
		}
	}
	*/
}

void SaveData::write_to_file(Level *level)
{
	// Create file in directory
	std::string path = "Content/Saves/" + saveName + ".txt";
	std::ofstream out(path);

	// Save main info
	out << saveName << "\n";

	out << currentStage << " " << currentLevel << "\n\n";

	// Save player info
	out << playerCount << "\n";
	for (int i = 0; i < playerCount; i++) {
		// Position
		out << level->players[i].gPos.x << " " << level->players[i].gPos.y << "\n";

		// Stats
		out << level->players[i].health << " " << level->players[i].armor << " " << level->players[i].gold << "\n";

		// Inventory
		out << "wooden_sword wooden_sword wooden_sword wooden_sword wooden_sword\n";
		/*
		for (int j = 0; j < 5; j++) {
			out << level->players[i].items[j].name;
			if (j != 4)
				out << " ";
			else
				out << "\n\n";
		}
		*/
	}

	// Save level data
	for (int i = 0; i < 12; i++) {
		out << "\n\n\n";

		// Save level dimensions
		out << levelData[i].width << " " << levelData[i].height << "\n\n";

		// Save player spawn locations
		out << levelData[i].playerStartPos.x << " " << levelData[i].playerStartPos.y << "\n\n";

		// Save portals
		out << levelData[i].portalCount << "\n";

		for (int j = 0; j < levelData[i].portalCount; j++) {
			out << levelData[i].portalPos[j].x	   << " " << levelData[i].portalPos[j].y	 << " ";
			out << levelData[i].portalExitPos[j].x << " " << levelData[i].portalExitPos[j].y << " ";
			out << levelData[i].portalNewLevel[j]  << "\n";
		}
		out << "\n";

		// Only update the level which was loaded
		if (level->currentStage * levelsInStage + level->currentLevel == i) {
			// Save walls
			int levelWallCount = 0;
			for (int j = 0; j < level->tiles.size(); j++)
				if (level->tiles[j].type == WALL)
					if (check_bounds(level->tiles[j].gridPos, RECT{ 1, 1, _LEVEL_WIDTH - 2, _LEVEL_HEIGHT - 2 }))
						++levelWallCount;

			out << levelWallCount << "\n";

			for (int j = 0; j < level->tiles.size(); j++) {
				if (level->tiles[j].type == WALL) {
					if (check_bounds(level->tiles[j].gridPos, RECT{ 1, 1, _LEVEL_WIDTH - 2, _LEVEL_HEIGHT - 2 })) {
						out << level->tiles[j].gridPos.x	<< " ";
						out << level->tiles[j].gridPos.y	<< " ";
						out << level->tiles[j].variant		<< " ";
						out << level->tiles[j].activatable	<< " ";
						out << level->tiles[j].toggleable	<< " ";
						out << level->tiles[j].active		<< "\n";
					}
				}
			}
			out << "\n";

			// Save floor
			out << levelData[i].baseFloorVariant << "\n";

			int levelFloorCount = 0;
			for (int j = 0; j < level->tiles.size(); j++)
				if (level->tiles[j].type == FLOOR)
					++levelFloorCount;

			out << levelFloorCount << "\n";

			for (int j = 0; j < level->tiles.size(); j++) {
				if (level->tiles[j].type == FLOOR) {
					out << level->tiles[j].gridPos.x << " " << level->tiles[j].gridPos.x << " ";
					out << level->tiles[j].variant	 << "\n";
				}
			}
		}
		else {
			// Save walls
			out << levelData[i].wallCount << "\n";

			for (int j = 0; j < levelData[i].wallCount; j++) {
				out << levelData[i].wallGridPos[j].x	<< " ";
				out << levelData[i].wallGridPos[j].y	<< " ";
				out << levelData[i].wallVariant[j]		<< " ";
				out << levelData[i].wallActivatable[j]	<< " ";
				out << levelData[i].wallToggleable[j]	<< " ";
				out << levelData[i].wallActive[j]		<< "\n";
			}
			out << "\n";

			// Save floor
			out << levelData[i].baseFloorVariant << "\n";

			out << levelData[i].floorCount << "\n";

			for (int j = 0; j < levelData[i].floorCount; j++) {
				out << levelData[i].floorGridPos[j].x << " " << levelData[i].floorGridPos[j].y << " ";
				out << levelData[i].floorVariant[j]	  << "\n";
			}
		}
	}
}