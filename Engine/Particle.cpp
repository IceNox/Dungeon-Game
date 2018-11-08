#include "Particle.h"

#include <cmath>

Particle::Particle
(
	Pos2D cPos,
	Pos2D vVec,
	Pos2D aVec,
	Sprite s,
	int eLength,
	int fLength
) :
	cPos(cPos),
	vVec(vVec),
	aVec(aVec),
	s(s),
	existenceLength(eLength),
	fadeLength(fLength)
{
	rcPos.x = cPos.x * 1000;
	rcPos.y = cPos.y * 1000;

	rvVec.x = vVec.x * 1000;
	rvVec.y = vVec.y * 1000;

	creationTime = maintime::now();
	lastUpdated  = maintime::now();

	transparency = 0.0f;

	finished = false;
}

Particle::Particle
(
	Pos2D cPos,
	std::string text
) :
	cPos(cPos),
	vVec(Pos2D(0, -32)),
	aVec(Pos2D(0,  32)),
	text(text),
	isText(true),
	existenceLength(1000),
	fadeLength(1000)
{
	rcPos.x = cPos.x * 1000;
	rcPos.y = cPos.y * 1000;

	rvVec.x = vVec.x * 1000;
	rvVec.y = vVec.y * 1000;

	creationTime = maintime::now();
	lastUpdated  = maintime::now();

	transparency = 0.0f;

	finished = false;
}

void Particle::update_particle()
{
	if (finished) return;

	using namespace std::chrono;

	// Position calculation
	long int usElapsed = (maintime::now() - lastUpdated).get_duration();

	rvVec.x += aVec.x * usElapsed / 1000.f;
	rvVec.y += aVec.y * usElapsed / 1000.f;

	rcPos.x += round(rvVec.x * (usElapsed / 1000000.0f));
	rcPos.y += round(rvVec.y * (usElapsed / 1000000.0f));

	cPos.x = round(rcPos.x / 1000.0f);
	cPos.y = round(rcPos.y / 1000.0f);
	
	// Update time
	lastUpdated = maintime::now();

	// Fade calculation
	long int msElapsed = (maintime::now() - creationTime).get_duration(MILLISECONDS);

	if (msElapsed > existenceLength) {
		transparency = 1.0f;
		finished	 = true;
	}
	if (msElapsed < existenceLength - fadeLength) return;

	transparency = 1.0f - (msElapsed - existenceLength + fadeLength) / (float)fadeLength;
	transparency *= transparency;
}
