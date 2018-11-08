#pragma once

#ifndef TEXTURES_H
#define TEXTURES_H

#include <vector>
#include "Pixel.h"

class Textures
{
public:
	Textures(std::vector<Pixel> addPixels, std::string name);

	std::string name;

	int width;
	int height;

	std::vector<Pixel> pixels;
};

#endif // TEXTURES_H