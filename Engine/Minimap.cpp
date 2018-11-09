#include "Minimap.h"

void Minimap::create_minimap(int width, int height)
{
    this->width  = width;
    this->height = height;

    for (int i = 0; i < width * height; i++) {
        grid.push_back(0);
    }

    size = 1;
}

void Minimap::change_size()
{
    if        (size == 0) {
        size = 1;
    }
    else if (size == 1) {
        size = 2;
    }
    else if (size == 2) {
        size = 0;
    }
}

int Minimap::get_size()
{
    return size;
}