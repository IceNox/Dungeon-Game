#pragma once

#include <vector>
#include <string>
#include "MenuElement.h"

class MenuScene
{
public:
    std::string title;
    std::string backgroundSpriteName;

    std::vector<Element> elements;
	
	int aElements; // Total number of elements
    int sElements; // Number of selectable elements

	MenuScene
	(
		std::string title,
		std::string backgroundSpriteName,
		int aElements,
		int sElements
	);
};
