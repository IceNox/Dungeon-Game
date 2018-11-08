#pragma once

#include "MenuScene.h"

#include "Constants.h"
#include "GameTime.h"

#include "GameMessage.h"

#include "Keys.h"
#include "KeyBindings.h"

#include "UserData.h"

#include <string>

using namespace std::chrono;

class Menu
{
public:
	/*
    bool enterIsPressed = false;
    bool escapeIsPressed = false;
    bool upIsPressed = false;
    bool downIsPressed = false;
	bool leftIsPressed = false;
	bool rightIsPressed = false;
	*/

    std::vector<MenuScene> scenes;
    int cScene = 0;
	int cElement = 0;

	bool locked = false;

	int scrollAmount = 0;

	// elapsed time / entire time
	// (curtime - starttime) / (endtime - starttime) = %of time elapsed

/// Animation variables
	// Change element
	const int ECAlength = 150;
	bool inECA = false;
	float ECAprogress;
	//time_point<system_clock> ECAstart;
	TimePoint ECAstart;
	int ECAelement1;
	int ECAelement2;

	// Change scene
	const int SCAlength = 400;
	bool inSCA = false;
	float SCAprogress;
	//time_point<system_clock> SCAstart;
	TimePoint SCAstart;
	int SCAelement1;
	int SCAelement2;
	int SCAscene1;
	int SCAscene2;
	std::string SCAdirection;

	// Fade in animation
	const int FIAlength = 250;
	bool inFIA = false;
	float FIAprogress;
	//time_point<system_clock> FIAstart;
	TimePoint FIAstart;

	// Fade out animation
	const int FOAlength = 250;
	bool inFOA = false;
	float FOAprogress;
	//time_point<system_clock> FOAstart;
	TimePoint FOAstart;
	GameMessage* msg;

/// Functions
	void enter_menu(int scene = 0, int element = 0);
	void exit_menu(GameMessage* msg);
	//void enter_game(std::string newMode, LevelType type, std::string name);

    void update_menu(std::vector<GameMessage*> &msg, Keys &keys, UserData &userData);
	void menu_control(std::vector<GameMessage*> &msg, std::string action, UserData &userData, Keys &keys);
};
