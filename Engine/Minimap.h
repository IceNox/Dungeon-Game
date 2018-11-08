#pragma once

#include "Sprite.h"

#include <vector>

class Minimap
{
	int width;
	int height;

	int size;
public:
	std::vector<int> grid;

	std::vector<Sprite> cells;

	Minimap() {};
	void create_minimap(int height, int width);
	void change_size();
	int get_size();
};