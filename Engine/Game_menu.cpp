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
    TextRenderData txr;
    txr.centered = true;
    txr.font = "basic-large";
    txr.brightness = menuBrightness;

    render_text(Pos2D(centerX, 32), menu.scenes[menu.cScene].title, txr);

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
        volatile float eBrightness = 1.0f;
        if (menu.scenes[menu.cScene].elements[i].active == false) {
            eBrightness *= 0.5f;
        }

        // Evaluate x and y coordinates
        int x = menu.scenes[menu.cScene].elements[i].cX + centerX;
        int y = menu.scenes[menu.cScene].elements[i].cY;
        if (!menu.scenes[menu.cScene].elements[i].screenCentered) x -= centerX;

        // Draw selection marker if nescessary
        if (selected) {
            SpriteDrawData sdd;
            sdd.brightness = eBrightness * menuBrightness;
            sdd.transparency = selectionTransparency;

            if (menu.scenes[menu.cScene].elements[i].smtype == SM_LINE) {
                gfx.DrawSprite(Pos2D(x - 64, y + 24), sprites[SELECTION_MARKER], sdd);
            }
            else if (menu.scenes[menu.cScene].elements[i].smtype == SM_ARROW) {
                gfx.DrawSprite(Pos2D(x - 16, y), sprites[SELECTION_ARROW_RIGHT], sdd);
            }
        }

        // Draw the elements
        SpriteDrawData sdd;
        TextRenderData txr;
        sdd.brightness = eBrightness * menuBrightness;
        txr.brightness = eBrightness * menuBrightness;

        if (menu.scenes[menu.cScene].elements[i].type == ME_BUTTON || menu.scenes[menu.cScene].elements[i].type == ME_TEXT) {
            // Draw the button/text
            txr.centered = menu.scenes[menu.cScene].elements[i].coordCentered;

            render_text(Pos2D(x, y), menu.scenes[menu.cScene].elements[i].text, txr);
        }
        else if (menu.scenes[menu.cScene].elements[i].type == ME_SLIDER) {
            // Draw the slider
            if (menu.scenes[menu.cScene].elements[i].coordCentered) x -= 42;
            gfx.DrawSprite(Pos2D(x, y + 8), sprites[SLIDER_BODY], sdd);

            int value = menu.scenes[menu.cScene].elements[i].value;
            gfx.DrawSprite(Pos2D(x - 4 + value * 8, y + 4), sprites[SLIDER_HEAD], sdd);
        }
        else if (menu.scenes[menu.cScene].elements[i].type == ME_SELECTION) {
            // Draw the selection
            txr.centered = true;

            int w = menu.scenes[menu.cScene].elements[i].width;
            if (!menu.scenes[menu.cScene].elements[i].coordCentered) x += w / 2;

            render_text(Pos2D(x, y), menu.scenes[menu.cScene].elements[i].selections[menu.scenes[menu.cScene].elements[i].cSelection], txr);

            x -= w / 2;
            gfx.DrawSprite(Pos2D(x, y), sprites[SELECTION_ARROW_LEFT], sdd);

            x += w - 12;
            gfx.DrawSprite(Pos2D(x, y), sprites[SELECTION_ARROW_RIGHT], sdd);
        }
        else if (menu.scenes[menu.cScene].elements[i].type == ME_SWITCH) {
            // Draw the switch
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

            int w = 116;
            if (!menu.scenes[menu.cScene].elements[i].coordCentered) x += w / 2;

            txr.centered = true;

            render_text(Pos2D(x, y), text, txr);

            x -= w / 2;
            gfx.DrawSprite(Pos2D(x, y), sprites[SELECTION_ARROW_LEFT], sdd);

            x += w - 12;
            gfx.DrawSprite(Pos2D(x, y), sprites[SELECTION_ARROW_RIGHT], sdd);
        }
    }
}
