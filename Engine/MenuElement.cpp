#include "MenuElement.h"

Element::Element // Text and button constructor
(
    MenuElementType type,
    std::string text,
    bool screenCentered,
    bool coordCentered,
    MenuSelectionMarkerType smtype,
    int cX,
    int cY,
    bool active
)    :
    type(type),
    text(text),
    screenCentered(screenCentered),
    coordCentered(coordCentered),
    smtype(smtype),
    cX(cX),
    cY(cY),
    active(active),
    visible(true)
{}

Element::Element // Picture constructor
(
    std::string spriteName,
    bool screenCentered,
    bool coordCentered,
    MenuSelectionMarkerType smtype,
    int cX,
    int cY,
    bool active
) :
    type(ME_PICTURE),
    spriteName(spriteName),
    screenCentered(screenCentered),
    coordCentered(coordCentered),
    smtype(smtype),
    cX(cX),
    cY(cY),
    active(active),
    visible(true)
{}

Element::Element // Slider constructor
(
    bool screenCentered,
    bool coordCentered,
    MenuSelectionMarkerType smtype,
    int cX,
    int cY,
    bool active
)    :
    type(ME_SLIDER),
    screenCentered(screenCentered),
    coordCentered(coordCentered),
    smtype(smtype),
    cX(cX),
    cY(cY),
    active(active),
    visible(true)
{
    value = 0;
}

Element::Element // Selection constructor
(
    std::vector<std::string> selections,
    int width,
    bool screenCentered,
    bool coordCentered,
    MenuSelectionMarkerType smtype,
    int cX,
    int cY,
    bool active
) :
    type(ME_SELECTION),
    selections(selections),
    width(width),
    screenCentered(screenCentered),
    coordCentered(coordCentered),
    smtype(smtype),
    cX(cX),
    cY(cY),
    active(active),
    visible(true)
{
    cSelection = 0;
}

Element::Element // Switch constructor
(
    int variant,
    bool screenCentered,
    bool coordCentered,
    MenuSelectionMarkerType smtype,
    int cX,
    int cY,
    bool active
)    :
    type(ME_SWITCH),
    variant(variant),
    screenCentered(screenCentered),
    coordCentered(coordCentered),
    smtype(smtype),
    cX(cX),
    cY(cY),
    active(active),
    visible(true)
{
    on = true;
}
