#pragma once

#include "Command.h"
#include "GameTime.h"

#include "ScreenAnimations.h"
#include "Particle.h"

#include <string>
#include <vector>
#include <chrono>

using namespace std::chrono;

class StatusEffects
{
public:
	bool slowed;
	bool snared;
	bool stunned;
	bool burning;
	bool cursed;

	int  slowDuration;
	int  stunDuration;
	int snareDuration;
	int  burnDuration; // In ticks
	int curseDuration;

	float  slowStrength;
	int    burnStrength;
	int	  curseStrength;

	TimePoint  slowStart;
	TimePoint snareStart;
	TimePoint  stunStart;
	TimePoint curseStart;

	TimePoint burnLastTick;
	int		  burnTicksRem;
	int		  fireDamage;

	StatusEffects();

	void update_effects();

	void slow (int duration, float strength);
	void snare(int duration				   );
	void stun (int duration				   );
	void burn (int duration, int   strength);
	void curse(int duration, int   strength);

	void stop_slow ();
	void stop_snare();
	void stop_stun ();
	void stop_burn ();
	void stop_curse();
	void stop_effects();
};