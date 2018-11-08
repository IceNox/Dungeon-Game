#include "Functions.h"
#include "Constants.h"

#include "Sign.h"

Sign::Sign(int gX, int gY, std::string text) : gX(gX), gY(gY), rawText(text), playerNearby(false), visible(false)
{
	sX = gX * cellSize + cellSize / 2 - 36;
	sY = gY * cellSize + cellSize / 2 - 76;

	textLine.push_back("");

	if (rawText.size() < 1) return;
	rawText.erase(rawText.begin());
	if (rawText.size() < 1) return;
	

	for (int i = 0; i < rawText.size() - 1; i++) {
		if (rawText[i] == '/' && rawText[i+1] == 'n') {
			textLine.push_back("");
			i++;
			continue;
		}

		textLine[textLine.size() - 1] += rawText[i];
	}

	textLine[textLine.size() - 1] += rawText[rawText.size() - 1];

	revealed = false;
}

void Sign::update_sign(Player player)
{
	if (distance_manh(Pos2D(player.gX, player.gY), Pos2D(gX, gY)) <= 1) 
		playerNearby = true;
	else 
		playerNearby = false;
}