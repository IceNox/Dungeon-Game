#include "Entity.h"

#include <sstream>

Entity::Entity // Constructor for a regular entity
(
	Pos2D cPos,
	Pos2D vVec,
	Pos2D sOffset,
	Pos2D rangeVec,
	int			damage,
	int			heal,
	int			hitboxRadius,
	bool		cwp,
	bool		cwa,
	bool		animated,
	SpriteName	spriteName,
	int			sW,
	int			sH,
	int			frameCount,
	int			frameHoldTime
)	:
	cPos(cPos),
	vVec(vVec),
	sOffset(sOffset),
	rangeVec(rangeVec),
	damage(damage),
	heal(heal),
	hitboxRadius(hitboxRadius),
	collidesWithPlayer(cwp),
	collidesWithAI(cwa),
	animated(animated),
	spriteName(spriteName),
	sWidth(sW),
	sHeight(sH),
	frameCount(frameCount),
	frameHoldTime(frameHoldTime)
{
	gPos = cPos / cellSize;
	sPos = cPos - sOffset;

	scPos = cPos;

	item = false;
	gold = false;

	itemName = "";

	goldAmount = 0;

	currentFrame = 0;

	spriteRegion = { 0, 0, sWidth, sHeight };

	revealed = false;
	visible  = false;

	creationTime	= maintime::now();
	lastMove		= maintime::now();
	lastFrameChange = maintime::now();
}

Entity::Entity // Constructor for gold entity
(
	Pos2D gPos,
	int	goldAmount
)	:
	gPos(gPos),
	goldAmount(goldAmount)
{
	gold = true;
	item = false;

	collidesWithPlayer = true;
	collidesWithAI	   = false;

	cPos = gPos * cellSize;
	cPos.x += cellSize / 2;
	cPos.y += cellSize / 2 - 1;

	if (goldAmount > 0) {
		spriteName = GOLD_1;

		hitboxRadius = 12;
		sOffset = Pos2D(-12, -12);
	}
	if (goldAmount > 1) {
		spriteName = GOLD_2;

		hitboxRadius = 20;
		sOffset = Pos2D(-20, -16);
	}
	if (goldAmount > 2) {
		spriteName = GOLD_3;

		hitboxRadius = 20;
		sOffset = Pos2D(-20, -24);
	}
	if (goldAmount > 4) {
		spriteName = GOLD_4;

		hitboxRadius = 24;
		sOffset = Pos2D(-24, -28);
	}
	if (goldAmount > 9) {
		spriteName = GOLD_5;

		hitboxRadius = 28;
		sOffset = Pos2D(-28, -36);
	}
	if (goldAmount > 19) {
		spriteName = GOLD_6;

		hitboxRadius = 28;
		sOffset = Pos2D(-28, -36);
	}
	if (goldAmount > 49) {
		spriteName = GOLD_7;

		hitboxRadius = 40;
		sOffset = Pos2D(-40, -48);
	}

	sPos = cPos + sOffset;

	visible  = false;
	revealed = false;
	animated = false;
}

Entity::Entity // Constructor for item entity
(
	Pos2D gPos,
	SpriteName itemName
)   :
	gPos(gPos),
	spriteName(itemName)
{
	gold = false;
	item = true;

	collidesWithPlayer = true;
	collidesWithAI = false;

	cPos = gPos * cellSize;
	cPos.x += cellSize / 2;
	cPos.y += cellSize / 2 - 1;

	sOffset = Pos2D(-32, -48);

	sPos = cPos + sOffset;

	visible = false;
	revealed = false;
	animated = false;
}

void Entity::update_entity()
{
	if (!item && !gold) {
		// Move the entity
		cPos = cPos + vVec * ((maintime::now() - lastMove).get_duration() / 1000000.0f);
		gPos = cPos / cellSize;
		sPos = cPos + sOffset;

		//if ((cX + cellWidth / 2) / cellWidth < gX || (cX + cellWidth / 2) / cellWidth > gX) gX = cX / cellWidth;
		//if ((cY + cellHeight / 2) / cellHeight < gY || (cY + cellHeight / 2) / cellHeight > gY) gY = cY / cellHeight;
		lastMove = maintime::now();

		// Update animation
		if (!animated) return;

		int microsecondsElapsed	   = (maintime::now() - lastFrameChange).get_duration();
		int holdTimeInMicroseconds = frameHoldTime * 1000;

		while (microsecondsElapsed > holdTimeInMicroseconds) {
			currentFrame = (currentFrame + 1) % frameCount;

			spriteRegion.left  =  currentFrame		* sWidth;
			spriteRegion.right = (currentFrame + 1) * sWidth;
			/*
			std::string spriteNumber = "_";
			std::stringstream ss;
			ss << currentFrame + 1;
			spriteNumber += ss.str();

			spriteName = baseSpriteName + spriteNumber;
			*/

			lastFrameChange		+= Duration(holdTimeInMicroseconds);
			microsecondsElapsed -= holdTimeInMicroseconds;
		}
	}
}