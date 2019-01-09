#pragma once

#include "ChiliWin.h"
#include "Colors.h"

#include <string>

class Sprite
{
public:
    Sprite() {}
    Sprite(const std::string& filename, const std::string& spriteName);
    Sprite(int width, int height, std::string spriteName, int frames, int centerx, int centery);
    Sprite(const Sprite&);
    ~Sprite();
    Sprite& operator=(const Sprite&);
    void PutPixel(int x, int y, Color c);
    void PutPixel(int i, Color c);
    void SetCenterX(int x);
    void SetCenterY(int y);
    void SetCenterPos(int x, int y);
    void RotateClockwise(int times);
    void FlipHorizontal();
    void FlipVertical();
    void Upscale(int ratio);
    void ApplyTransparency(float t);
    Color GetPixel(int id) const;
    Color GetPixel(int x, int y) const;
    Color GetPixel(int x, int y, float brightness) const;
    int GetWidth() const;
    int GetHeight() const;
    int GetFrames() const;
    int GetCenterX() const;
    int GetCenterY() const;
    std::string GetName() const;
private:
    std::string spriteName;
    Color* pPixels = nullptr;
    int width;
    int height;
    int frames = 1;
    int centerX;
    int centerY;
};

Sprite get_sprite_region(Sprite s, RECT r);
Sprite create_rectangle(int w, int h, Color c);
//Sprite create_circle(int r, Color c, bool C = false);
