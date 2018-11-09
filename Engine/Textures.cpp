#include "Textures.h"

Textures::Textures(std::vector<Pixel> addPixels, std::string name)
{
    for (int i = 0; i < width * height; i++) {
        pixels.push_back(Pixel(addPixels[i].r, addPixels[i].g, addPixels[i].b, addPixels[i].transparent));
    }

    this->name = name;
}