#include "Menu.h"

#include "GlobalData.h"

#include <iostream>
#include <cmath>
#include <windows.h>
#include <fstream>
#include <sstream>

using namespace std::chrono;

void Menu::enter_menu(int scene, int element)
{
    cScene     = scene;
    cElement = element;

    inFIA     = true;
    FIAstart = maintime::now();

    inECA = false;
    inSCA = false;
    inFOA = false;
}

void Menu::exit_menu(GameMessage* msg)
{
    inFOA      = true;
    FOAstart  = maintime::now();
    this->msg = msg;
}
/*
void Menu::enter_game(std::string newMode, LevelType lType, std::string lName)
{
    inFOA     = true;
    FOAstart = maintime::now();
    this->newMode = newMode;
    this->lType      = lType;
    this->lName   = lName;
}
*/
void Menu::update_menu(std::vector<GameMessage*> &msg, Keys &keys, UserData &userData)
{
    using namespace std::chrono;

    if (inFIA) {
        //duration<float> elapsed_seconds = system_clock::now() - FIAstart;
        //float FIAtimeElapsed = elapsed_seconds.count();
        //FIAprogress = FIAtimeElapsed / FIAlength;

        int elapsedms = (maintime::now() - FIAstart).get_duration(MILLISECONDS);
        FIAprogress = elapsedms / (float)FIAlength;

        if (FIAprogress >= 1.0f) inFIA = false;

        return;
    }
    if (inFOA) {
        //duration<float> elapsed_seconds = system_clock::now() - FOAstart;
        //float FOAtimeElapsed = elapsed_seconds.count();
        //FOAprogress = FOAtimeElapsed / FOAlength;

        int elapsedms = (maintime::now() - FOAstart).get_duration(MILLISECONDS);
        FOAprogress = elapsedms / (float)FOAlength;

        if (FOAprogress >= 1.0f) {
            msg.push_back(this->msg);
        }

        return;
    }

    bool enter    = false;
    bool escape = false;
    bool up        = false;
    bool down    = false;
    bool left    = false;
    bool right    = false;

    if (keys.key_state(userData.keyBindings.ENTER )) enter  = true;
    if (keys.key_state(userData.keyBindings.ESCAPE)) escape = true;
    if (keys.key_state(userData.keyBindings.UP    )) up        = true;
    if (keys.key_state(userData.keyBindings.DOWN  )) down   = true;
    if (keys.key_state(userData.keyBindings.LEFT  )) left   = true;
    if (keys.key_state(userData.keyBindings.RIGHT )) right  = true;

    if (locked) {
        menu_control(msg, "", userData, keys);
        enter  = false;
        escape = false;
        up     = false;
        down   = false;
        left   = false;
        right  = false;
    }

    if (!inSCA) {
        if (enter) {
            if (scenes[cScene].elements[cElement].active) {
                SCAscene1 = cScene;
                SCAelement1 = cElement;
                menu_control(msg, "enter", userData, keys);
                SCAscene2 = cScene;
                SCAelement2 = cElement;

                if (SCAscene1 != SCAscene2) {
                    cScene     = SCAscene1;
                    cElement = SCAelement1;

                    inSCA         = true;
                    SCAstart     = maintime::now();
                    SCAdirection = "right";
                }
            }

        }
        else if (escape) {
            SCAscene1    = cScene;
            SCAelement1 = cElement;
            menu_control(msg, "back", userData, keys);
            SCAscene2    = cScene;
            SCAelement2 = cElement;

            if (SCAscene1 != SCAscene2) {
                cScene     = SCAscene1;
                cElement = SCAelement1;

                inSCA         = true;
                SCAstart     = maintime::now();
                SCAdirection = "left";
            }
        }
        else if (up) {
            if (cElement > 0) {
                ECAelement1 = cElement;
                cElement--;
                ECAelement2 = cElement;

                inECA = true;
                ECAstart = maintime::now();
                //ECAstart = system_clock::now();
            }
            else {
                ECAelement1 = cElement;
                cElement = scenes[cScene].sElements - 1;
                ECAelement2 = cElement;

                inECA = true;
                ECAstart = maintime::now();
                //ECAstart = system_clock::now();
            }
        }
        else if (down) {
            if (cElement < scenes[cScene].sElements - 1) {
                ECAelement1 = cElement;
                cElement++;
                ECAelement2 = cElement;

                inECA = true;
                ECAstart = maintime::now();
                //ECAstart = system_clock::now();
            }
            else {
                ECAelement1 = cElement;
                cElement = 0;
                ECAelement2 = cElement;

                inECA = true;
                ECAstart = maintime::now();
                //ECAstart = system_clock::now();
            }
        }
        else if (left) {
            menu_control(msg, "left", userData, keys);
        }
        else if (right) {
            menu_control(msg, "right", userData, keys);
        }
    }

    if (inECA) {
        //duration<float> elapsed_seconds = system_clock::now() - ECAstart;
        //float ECAtimeElapsed = elapsed_seconds.count();
        //ECAprogress = ECAtimeElapsed / ECAlength;

        int elapsedSeconds = (maintime::now() - ECAstart).get_duration(MILLISECONDS);
        ECAprogress = elapsedSeconds / (float) ECAlength;

        if (ECAprogress >= 1.0f) {
            inECA = false;
        }
    }

    if (inSCA) {
        //duration<float> elapsed_seconds = system_clock::now() - SCAstart;
        //float SCAtimeElapsed = elapsed_seconds.count();
        //SCAprogress = SCAtimeElapsed / SCAlength;

        int elapsedSeconds = (maintime::now() - SCAstart).get_duration(MILLISECONDS);
        SCAprogress = elapsedSeconds / (float)SCAlength;

        if (SCAprogress > 0.5f) {
            cScene = SCAscene2;
            cElement = SCAelement2;
            scrollAmount = 0;
        }

        if (SCAprogress >= 1.0f) {
            inSCA = false;
        }
    }
}

void Menu::menu_control(std::vector<GameMessage*> &msg, std::string action, UserData &userData, Keys &keys)
{
    std::ofstream debug("debug.txt");

    debug << cScene << " " << cElement << std::endl;

    // Scene 0
    if (cScene == 0) {
        if (action == "back") {
            return;
        }
        // Element 0
        if (cElement == 0) {
            if (action == "enter") {
                cScene = 1;
                cElement = 0;
                return;
            }
        }

        // Element 1
        if (cElement == 1) {
            if (action == "enter") {
                cScene = 4;
                cElement = 0;
                return;
            }
        }

        // Element 2
        if (cElement == 2) {
            if (action == "enter") {
                // Exit the program
                exit(0);
            }
        }
    }

    // Scene 1
    else if (cScene == 1) {
        if (action == "back") {
            cScene = 0;
            cElement = 0;
            return;
        }
        // Element 0
        if (cElement == 0) {
            if (action == "enter") {
                // Start new game
                return;
            }
        }
        // Element 1
        if (cElement == 1) {
            if (action == "enter") {
                cScene = 2;
                cElement = 0;
                return;
            }
        }
        // Element 2
        if (cElement == 2) {
            if (action == "enter") {
                cScene = 3;
                cElement = 0;
                return;
            }
        }
    }

    // Scene 2
    else if (cScene == 2) {
        if (action == "back") {
            cScene = 1;
            cElement = 1;
            return;
        }
        // Elements
        if (action == "enter") {
            exit_menu(new MsgEnterLevel(SAVE, _SAVE_FILE_NAMES[cElement]));
        }
    }

    // Scene 3
    else if (cScene == 3) {
        if (action == "back") {
            cScene = 1;
            cElement = 2;
            return;
        }
        // Elements
        if (action == "enter") {
            exit_menu(new MsgEnterLevel(USER, _USER_LEVEL_FILE_NAMES[cElement]));
        }
    }

    // Scene 4
    else if (cScene == 4) {
        if (action == "back") {
            cScene = 0;
            cElement = 1;
            return;
        }
        // Element 0
        if (cElement == 0) {
            if (action == "enter") {
                scenes[5].elements[7].visible = false;
                scenes[5].elements[8].visible = false;
                cScene = 5;
                cElement = 0;
                return;
            }
        }

        // Element 1
        if (cElement == 1) {
            if (action == "enter") {
                cScene = 6;
                cElement = 0;
                return;
            }
        }
    }

    // Scene 5
    else if (cScene == 5) {
        if (action == "back") {
            cScene = 4;
            cElement = 0;
            return;
        }
        // Element 0
        if (cElement == 0) {
            if (action == "enter" || action == "left" || action == "right") {
                scenes[cScene].elements[cElement].on = abs(scenes[cScene].elements[cElement].on - 1);
                userData.settings.fullscreen = scenes[cScene].elements[cElement].on;
                scenes[5].elements[7].visible = false;
                scenes[5].elements[8].visible = false;
                return;
            }
        }
        // Element 1
        if (cElement == 1) {
            int &cs = scenes[cScene].elements[cElement].cSelection;
            int ss = scenes[cScene].elements[cElement].selections.size();

            bool r = true;

            if (action == "left") {
                if (cs > 0) cs--;
                else cs = ss - 1;
                r = false;
            }
            else if (action == "right" || action == "enter") {
                if (cs < ss - 1) cs++;
                else cs = 0;
                r = false;
            }

            if (r) return;

            std::string ws = "";
            std::string hs = "";
            for (int i = 0; i < scenes[cScene].elements[cElement].selections[cs].length(); i++) {
                if (scenes[cScene].elements[cElement].selections[cs][i] != 'x') {
                    ws += scenes[cScene].elements[cElement].selections[cs][i];
                }
                else {
                    for (int j = i + 1; j < scenes[cScene].elements[cElement].selections[cs].length(); j++) {
                        hs += scenes[cScene].elements[cElement].selections[cs][j];
                        i = scenes[cScene].elements[cElement].selections[cs].length();
                    }
                }
            }

            std::istringstream w(ws);
            std::istringstream h(hs);
            w >> userData.settings.window_width;
            h >> userData.settings.window_height;

            scenes[5].elements[7].visible = false;
            scenes[5].elements[8].visible = false;

            /*
            if        (cs == 0) {
                userData.settings.window_width  = 1280;
                userData.settings.window_height = 720;
            }
            else if (cs == 1) {
                userData.settings.window_width  = 1280;
                userData.settings.window_height = 1024;
            }
            else if (cs == 2) {
                userData.settings.window_width  = 1440;
                userData.settings.window_height = 900;
            }
            else if (cs == 3) {
                userData.settings.window_width  = 1600;
                userData.settings.window_height = 900;
            }
            else if (cs == 4) {
                userData.settings.window_width  = 1920;
                userData.settings.window_height = 1080;
            }
            else if (cs == 5) {
                userData.settings.window_width  = 2560;
                userData.settings.window_height = 1440;
            }
            else if (cs == 6) {
                userData.settings.window_width  = 2560;
                userData.settings.window_height = 1600;
            }
            */
        }
        else if (cElement == 2) {
            if (action == "enter" || action == "left" || action == "right") {
                scenes[cScene].elements[cElement].on = abs(scenes[cScene].elements[cElement].on - 1);
                userData.settings.native = scenes[cScene].elements[cElement].on;
                scenes[5].elements[7].visible = false;
                scenes[5].elements[8].visible = false;
                return;
            }
        }
        else if (cElement == 3) {
            if (action == "enter") {
                userData.save_data();
                scenes[5].elements[7].visible = true;
                scenes[5].elements[8].visible = true;
            }
        }
    }

    // Scene 6
    else if (cScene == 6) {
        if (action == "back") {
            cScene = 4;
            cElement = 1;
            return;
        }

        if (locked) {
            int code;
            if (keys.pressed_key_code(code)) {
                if        (cElement == 0)  userData.keyBindings.MOVE_UP            = code;
                else if (cElement == 1)  userData.keyBindings.MOVE_DOWN            = code;
                else if (cElement == 2)  userData.keyBindings.MOVE_LEFT            = code;
                else if (cElement == 3)  userData.keyBindings.MOVE_RIGHT        = code;
                else if (cElement == 4)  userData.keyBindings.CHANGE_DIRECTION    = code;
                else if (cElement == 5)  userData.keyBindings.CHANGE_MAP_SIZE    = code;
                else if (cElement == 6)  userData.keyBindings.PAUSE                = code;
                else if (cElement == 7)  userData.keyBindings.USE_ITEM            = code;
                else if (cElement == 8)  userData.keyBindings.DROP_ITEM            = code;
                else if (cElement == 9)  userData.keyBindings.ITEM_1            = code;
                else if (cElement == 10) userData.keyBindings.ITEM_2            = code;
                else if (cElement == 11) userData.keyBindings.ITEM_3            = code;
                else if (cElement == 12) userData.keyBindings.ITEM_4            = code;
                else if (cElement == 13) userData.keyBindings.ITEM_5            = code;

                scenes[cScene].elements[cElement].text = keys.key_name(code);

                userData.save_data();
                locked = false;
            }
        }
        if (!locked) {
            if (action == "enter") {
                locked = true;
                scenes[cScene].elements[cElement].text = "press a key";
            }
        }
        /*
        // Element 0
        if (cElement == 0) {
            if (action == "enter") {
                return;
            }
        }*/
    }
}
