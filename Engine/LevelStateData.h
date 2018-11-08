#pragma once

#include <vector>
#include <string>

struct TileData
{
	// Wire stuff
	bool pressuredW;
	bool pressuredS;
	bool pressuredG;
	bool pressuredB;
	bool pressured;
	bool powered;
	
	// Occupation
	bool terrain;
	bool object;

	//
	bool player;
	bool enemy;

	bool directlyLit;
};

class LevelStateData
{
public:
	int width;
	int height;

	std::vector<TileData> tiles;



	LevelStateData() {};

	void set(int w, int h)
	{
		width  = w;
		height = h;

		tiles.clear();
		for (int i = 0; i < w * h; i++) {
			tiles.push_back(TileData());
		}

		reset();
	};

	void reset()
	{
		for (int i = 0; i < tiles.size(); i++) {
			tiles[i].pressuredW = false;
			tiles[i].pressuredS = false;
			tiles[i].pressuredG = false;
			tiles[i].pressuredB = false;
			tiles[i].powered = false;

			tiles[i].terrain = false;
			tiles[i].object	 = false;

			tiles[i].player = false;
			tiles[i].enemy	= false;

			tiles[i].directlyLit = false;
		}
	}
};