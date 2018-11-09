#pragma once

#ifndef PIXEL_H
#define PIXEL_H

class Pixel
{
public:
    Pixel(int r, int g, int b, bool transparent);

    int r;
    int g;
    int b;
    bool transparent;
};

#endif // PIXEL_H