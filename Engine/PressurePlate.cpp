#include "PressurePlate.h"

#include "GlobalData.h"
#include "Constants.h"

PressurePlate::PressurePlate(int gX, int gY, std::string type) : gX(gX), gY(gY), type(type)
{
	revealed = false;
	pressed = false;
}

void PressurePlate::update_plate(std::vector<Tile> &tiles)
{
	if		(tiles[gY * levelWidth + gX].pressuredW && type == "wooden") pressed = true;
	else if (tiles[gY * levelWidth + gX].pressuredS && type == "steel") pressed = true;
	else if (tiles[gY * levelWidth + gX].pressuredG && type == "gold") pressed = true;
	else pressed = false;

	if (!pressed) {
		sX = gX * cellSize + 20;
		sY = gY * cellSize + 8;
	}
	else if (pressed) {
		sX = gX * cellSize + 20;
		sY = gY * cellSize + 16;
	}
}