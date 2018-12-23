#pragma once

#include "ChiliWin.h"
#include "Colors.h"

#include <string>
#include <Windows.h>

class Sprite
{
public:
    Sprite() {}
    Sprite(const std::string& filename, const std::string& spriteName);
    Sprite(int width, int height, std::string spriteName);
    Sprite(const Sprite&);
    ~Sprite();
    Sprite& operator=(const Sprite&);
    void PutPixel(int x, int y, Color c);
    void PutPixel(int i, Color c);
    void RotateClockwise(int times);
    void Upscale(int ratio);
    void ApplyTransparency(float t);
    Color GetPixel(int id) const;
    Color GetPixel(int x, int y) const;
    Color GetPixel(int x, int y, float brightness) const;
    int GetWidth() const;
    int GetHeight() const;
    int GetFrames() const;
    std::string GetName() const;
private:
    std::string spriteName;
    Color* pPixels = nullptr;
    int width;
    int height;
    int frames = 1;
};

Sprite get_sprite_region(Sprite s, RECT r);
Sprite create_rectangle(int w, int h, Color c);
//Sprite create_circle(int r, Color c, bool C = false);
