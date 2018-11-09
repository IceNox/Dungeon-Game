#pragma once

#include "Constants.h"

#include <string>
#include <vector>

class Element
{
public:
    int cX;
    int cY;

    MenuElementType type;
    /*
    Element types:
        - picture (Plain picture, can't be selected)
        - text (Plain text, can't be selected)
        - button (When selected does something)
        - slider (Its value can be adjusted using left and right arrow keys)
        - selection (You can cycle through available options with left and right arrow keys)
        - switch (Can only be true or false, and is switched by pressing enter)
    */

    bool active;

    bool screenCentered;
    bool coordCentered;

    MenuSelectionMarkerType smtype; // Selection marker type

    bool visible;

    // Button/Text variables
    std::string text;

    // Picture variables
    std::string spriteName;

    // Slider variables
    int value;

    // Selection variables
    int cSelection;
    int width;
    std::vector<std::string> selections;

    // Switch variables
    bool on;
    int variant; // 0 - true/false, 1 - on/off, 2 - yes/no 

    Element // Text and button constructor
    (
        MenuElementType type,
        bool active,
        std::string text,
        bool screenCentered,
        bool coordCentered,
        MenuSelectionMarkerType smtype,
        int cX,
        int cY
    );

    Element // Picture constructor
    (
        std::string spriteName,
        bool screenCentered,
        bool coordCentered,
        MenuSelectionMarkerType smtype,
        int cX,
        int cY
    );

    Element // Slider constructor
    (
        bool screenCentered,
        bool coordCentered,
        MenuSelectionMarkerType smtype,
        int cX,
        int cY
    );

    Element // Selection constructor
    (
        std::vector<std::string> selections,
        int width,
        bool screenCentered,
        bool coordCentered,
        MenuSelectionMarkerType smtype,
        int cX,
        int cY
    );

    Element // Switch constructor
    (
        int variant,
        bool screenCentered,
        bool coordCentered,
        MenuSelectionMarkerType smtype,
        int cX,
        int cY
    );
};
