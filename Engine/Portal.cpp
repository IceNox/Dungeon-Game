#include "Portal.h"

Portal::Portal
(
	Pos2D entryP,
	Pos2D exitP,
	int nl
) : 
	entryPos(entryP),
	exitPos(exitP),
	newLevel(nl)
{
	if (nl < 0) {
		exit = true;
	}

	spriteRegion = { 0, 0, cellSize, cellSize / 2 };

	lastFrameChange = maintime::now();
	currentFrame	= 0;
}

void Portal::update_portal()
{
	int microsecondsElapsed = (maintime::now() - lastFrameChange).get_duration();
	int holdTimeInMicroseconds = frameHoldTime * 1000;

	while (microsecondsElapsed > holdTimeInMicroseconds) {
		currentFrame = (currentFrame + 1) % frameCount;

		spriteRegion.left  =  currentFrame		* cellSize;
		spriteRegion.right = (currentFrame + 1) * cellSize;

		lastFrameChange		+= Duration(holdTimeInMicroseconds);
		microsecondsElapsed -= holdTimeInMicroseconds;
	}
}