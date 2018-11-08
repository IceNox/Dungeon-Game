#include "MenuElement.h"

Element::Element // Text and button constructor
(
	MenuElementType type,
	bool active,
	std::string text,
	bool screenCentered,
	bool coordCentered,
	MenuSelectionMarkerType smtype,
	int cX,
	int cY
)	:
	type(type),
	active(active),
	text(text),
	screenCentered(screenCentered),
	coordCentered(coordCentered),
	smtype(smtype),
	cX(cX),
	cY(cY),
	visible(true)
{}

Element::Element // Picture constructor
(
	std::string spriteName,
	bool screenCentered,
	bool coordCentered,
	MenuSelectionMarkerType smtype,
	int cX,
	int cY
) :
	type(ME_PICTURE),
	spriteName(spriteName),
	screenCentered(screenCentered),
	coordCentered(coordCentered),
	smtype(smtype),
	cX(cX),
	cY(cY),
	visible(true)
{}

Element::Element // Slider constructor
(
	bool screenCentered,
	bool coordCentered,
	MenuSelectionMarkerType smtype,
	int cX,
	int cY
)	:
	type(ME_SLIDER),
	screenCentered(screenCentered),
	coordCentered(coordCentered),
	smtype(smtype),
	cX(cX),
	cY(cY),
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
	int cY
) :
	type(ME_SELECTION),
	selections(selections),
	width(width),
	screenCentered(screenCentered),
	coordCentered(coordCentered),
	smtype(smtype),
	cX(cX),
	cY(cY),
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
	int cY
)	:
	type(ME_SWITCH),
	variant(variant),
	screenCentered(screenCentered),
	coordCentered(coordCentered),
	smtype(smtype),
	cX(cX),
	cY(cY),
	visible(true)
{
	on = true;
}