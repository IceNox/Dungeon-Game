#include "Game.h"

#include "GlobalData.h"

#include "MainWindow.h"

#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>

void Game::ComposeMenu()
{
    // Set brightness
    float menuBrightness = 1.0f;
    if (menu.inFIA) menuBrightness =        menu.FIAprogress;
    if (menu.inFOA) menuBrightness = 1.0f - menu.FOAprogress;
    menuBrightness *= menuBrightness;

    /*
    // If fade out animation is done, enter game/editor
    if (menu.inFOA) {
        if (menu.FOAprogress > 1.0f) {


            if (menu.newMode == "game") {
                inGame = true;

                if (menu.lType == SAVE) {
                    level.load_save_file(menu.lName + ".txt");
                    level.update_level(screenAnimations, keys, userData);
                    levelLoaded = true;
                }
                else if (menu.lType == USER) {
                    level.load_level(menu.lName + ".txt");
                    level.update_level(screenAnimations, keys, userData);
                    levelLoaded = true;
                }
            }
            return;
        }
    }
    */

    // Set center x coordinate
    int centerX = gfx.ScreenWidth / 2;

    // If scene change animation is playing, evaluate new center x coordinate
    if (menu.inSCA) {
        if (menu.SCAprogress < 0.5) {
            if (menu.SCAdirection == "right") {
                centerX -= 2 * gfx.ScreenWidth * menu.SCAprogress * menu.SCAprogress;
            }
            else if (menu.SCAdirection == "left") {
                centerX += 2 * gfx.ScreenWidth * menu.SCAprogress * menu.SCAprogress;
            }
        }
        else {
            if (menu.SCAdirection == "right") {
                centerX += 2 * gfx.ScreenWidth * (1.0f - menu.SCAprogress) * (1.0f - menu.SCAprogress);
            }
            else if (menu.SCAdirection == "left") {
                centerX -= 2 * gfx.ScreenWidth * (1.0f - menu.SCAprogress) * (1.0f - menu.SCAprogress);
            }
        }
    }

    // Draw scene title text
    draw_text(Pos2D(centerX, 32), menu.scenes[menu.cScene].title, true, true, menuBrightness);

    // Draw scene elements
    draw_elements(centerX, menuBrightness);
}

void Game::draw_elements(int centerX, float menuBrightness)
{
    for (int i = 0; i < menu.scenes[menu.cScene].aElements; i++) {
        if (!menu.scenes[menu.cScene].elements[i].visible) continue;

        bool selected = false;
        float selectionTransparency = 1.0f;

        // Set selection marker variables
        if (menu.inECA) {
            if        (menu.ECAelement1 == i) {
                selected = true;
                selectionTransparency -= menu.ECAprogress;
            }
            else if (menu.ECAelement2 == i) {
                selected = true;
                selectionTransparency = menu.ECAprogress;
            }
        }
        else {
            if (menu.cElement == i) selected = true;
        }

        // Make the brightness change exponential
        selectionTransparency = selectionTransparency * selectionTransparency;

        // If the element is inactive, grey it out
        float eBrightness = 1.0f;
        if (menu.scenes[menu.cScene].elements[i].active == false) {
            eBrightness *= 0.5f;
        }

        // Evaluate x and y coordinates
        int x = menu.scenes[menu.cScene].elements[i].cX + centerX;
        int y = menu.scenes[menu.cScene].elements[i].cY;
        if (!menu.scenes[menu.cScene].elements[i].screenCentered) x -= centerX;

        // Draw selection marker if nescessary
        if (selected) {
            if (menu.scenes[menu.cScene].elements[i].smtype == SM_LINE) {
                send_draw_info(Pos2D(x - 64, y + 24), eBrightness * menuBrightness, sprites[SELECTION_MARKER], selectionTransparency);
            }
            else if (menu.scenes[menu.cScene].elements[i].smtype == SM_ARROW) {
                send_draw_info(Pos2D(x - 16, y), eBrightness * menuBrightness, sprites[SELECTION_ARROW_RIGHT], selectionTransparency);
            }
        }

        if (menu.scenes[menu.cScene].elements[i].type == ME_BUTTON || menu.scenes[menu.cScene].elements[i].type == ME_TEXT) {
            // Draw the button/text
            draw_text
            (
                Pos2D(x, y),
                menu.scenes[menu.cScene].elements[i].text,
                menu.scenes[menu.cScene].elements[i].coordCentered,
                false,
                eBrightness * menuBrightness
            );
        }
        else if (menu.scenes[menu.cScene].elements[i].type == ME_SLIDER) {
            // Draw the slider
            if (menu.scenes[menu.cScene].elements[i].coordCentered) x -= 42;
            send_draw_info(Pos2D(x, y + 8), eBrightness * menuBrightness, sprites[SLIDER_BODY]);

            int value = menu.scenes[menu.cScene].elements[i].value;
            send_draw_info(Pos2D(x - 4 + value * 8, y + 4), eBrightness * menuBrightness, sprites[SLIDER_HEAD]);
        }
        else if (menu.scenes[menu.cScene].elements[i].type == ME_SELECTION) {
            // Draw the selection
            int w = menu.scenes[menu.cScene].elements[i].width;

            if (!menu.scenes[menu.cScene].elements[i].coordCentered) x += w / 2;

            draw_text
            (
                Pos2D(x, y),
                menu.scenes[menu.cScene].elements[i].selections[menu.scenes[menu.cScene].elements[i].cSelection],
                true,
                false,
                eBrightness * menuBrightness
            );

            x -= w / 2;
            send_draw_info(Pos2D(x, y), 1.0f * menuBrightness, sprites[SELECTION_ARROW_LEFT]);

            x += w - 12;
            send_draw_info(Pos2D(x, y), 1.0f * menuBrightness, sprites[SELECTION_ARROW_RIGHT]);
        }
        else if (menu.scenes[menu.cScene].elements[i].type == ME_SWITCH) {
            // Draw the switch
            int w = 116;

            std::string text;

            if        (menu.scenes[menu.cScene].elements[i].variant == 0) {
                if (menu.scenes[menu.cScene].elements[i].on)
                    text = "true";
                else
                    text = "false";
            }
            else if (menu.scenes[menu.cScene].elements[i].variant == 1) {
                if (menu.scenes[menu.cScene].elements[i].on)
                    text = "on";
                else
                    text = "off";
            }
            else if (menu.scenes[menu.cScene].elements[i].variant == 2) {
                if (menu.scenes[menu.cScene].elements[i].on)
                    text = "yes";
                else
                    text = "no";
            }

            if (!menu.scenes[menu.cScene].elements[i].coordCentered) x += w / 2;

            draw_text
            (
                Pos2D(x, y),
                text,
                true,
                false,
                eBrightness * menuBrightness
            );

            x -= w / 2;
            send_draw_info(Pos2D(x, y), 1.0f * menuBrightness, sprites[SELECTION_ARROW_LEFT]);

            x += w - 12;
            send_draw_info(Pos2D(x, y), 1.0f * menuBrightness, sprites[SELECTION_ARROW_RIGHT]);
        }
    }
}
