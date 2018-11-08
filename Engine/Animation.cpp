#include "Animation.h"

#include <cmath>
#include <sstream>

Animation::Animation
(
	Pos2D cPos,
	bool g,
	SpriteName name,
	int sw,
	int sh,
	int ht,
	int fc
)	:
	cPos(cPos),
	ground(g),
	spriteName(name),
	sWidth(sw),
	sHeight(sh),
	frameHoldTime(ht),
	frameCount(fc)
{
	creationTime = maintime::now();
	currentFrame = 0;

	spriteRegion = { 0, 0, sWidth, sHeight };

	finished = false;
}

void Animation::update_animation()
{
	using namespace std::chrono;

	currentFrame = floor((maintime::now() - creationTime).get_duration(MILLISECONDS) / frameHoldTime);
	if (currentFrame >= frameCount) finished = true;
	else {
		spriteRegion.left  =  currentFrame		* sWidth;
		spriteRegion.right = (currentFrame + 1) * sWidth;
	}
}