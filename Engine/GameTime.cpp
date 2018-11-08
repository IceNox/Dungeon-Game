#include "GameTime.h"

long long int maintime::currentGameTime = 0;
long long int maintime::updateCount = 0;

time_point<high_resolution_clock> maintime::lastTimeSync = high_resolution_clock::now();
time_point<high_resolution_clock> maintime::currentTime  = high_resolution_clock::now();
float maintime::clockSpeed = 0.0f;
bool  maintime::paused     = 0;

void maintime::initialize()
{
	currentGameTime = 0;
	updateCount		= 0;
	currentTime		= high_resolution_clock::now();
	lastTimeSync	= currentTime;
	clockSpeed		= 1.0f;
	paused			= false;
}

void maintime::update_time()
{
	// Get current time
	currentTime = high_resolution_clock::now();

	/*
	if (updateCount == 1) {
		currentGameTime += 5;
		currentGameTime -= 5;
	}

	currentTime += microseconds(1);
	currentTime -= microseconds(1);
	lastTimeSync += microseconds(1);
	lastTimeSync -= microseconds(1);
	*/

	currentGameTime;

	// Update game time
	if (!paused) {
		
		currentGameTime += duration_cast<microseconds>(currentTime - lastTimeSync).count() * clockSpeed;
	}

	// TEST
	//_currentGameTime = currentGameTime;

	// Set the time of last synchronization
	lastTimeSync = currentTime;

	// Increment update count
	++updateCount;
}

bool maintime::set_clock_speed(float s)
{
	if (s <= 0.01f)
		return false;
	if (s > 10.0f)
		return false;

	clockSpeed = s;
	return true;
}

bool maintime::increase_clock_speed(float s)
{
	return set_clock_speed(clockSpeed + s);
}

bool maintime::decrease_clock_speed(float s)
{
	return set_clock_speed(clockSpeed - s);
}

void maintime::pause()
{
	paused = true;
}

void maintime::unpause()
{
	paused = false;
}

TimePoint maintime::now()
{
	return currentGameTime;
}