#include "MenuControlFunctions.h"
#include "Game.h"
#include <windows.h>
#include <iostream>
#include <fstream>

using namespace std;

void menu_control(int &scene, int &element, bool &inMenu, bool &inGame, string action)
{
	ofstream debug("debug.txt");

	debug << scene << " " << element << endl;

	// Scene 0
	if (scene == 0) {
		if (action == "back") {
			return;
		}
		// Element 0
		if (element == 0) {
			if (action == "enter") {
				inMenu = false;
				inGame = true;
				return;
			}
		}

		// Element 1
		if (element == 1) {
			if (action == "enter") {
				scene = 1;
				element = 0;
				return;
			}
		}

		// Element 2
		if (element == 2) {
			if (action == "enter") {
				// Exit the program
				exit(0);
			}
		}
	}

	// Scene 1
	else if (scene == 1) {
		if (action == "back") {
			scene = 0;
			element = 1;
			return;
		}
		// Element 0
		if (element == 0) {
			if (action == "enter") {
				scene = 2;
				element = 0;
				return;
			}
		}

		// Element 1
		if (element == 1) {
			if (action == "enter") {
				scene = 3;
				element = 0;
				return;
			}
		}
	}

	// Scene 2
	else if (scene == 2) {
		if (action == "back") {
			scene = 1;
			element = 0;
			return;
		}
		// Element 0
		if (element == 0) {
			if (action == "enter") {
				return;
			}
		}
	}

	// Scene 3
	else if (scene == 3) {
		if (action == "back") {
			scene = 1;
			element = 1;
			return;
		}
		// Element 0
		if (element == 0) {
			if (action == "enter") {
				return;
			}
		}
	}
}
