#include "Sprite.h"

#include "GlobalData.h"
#include "Functions.h"

#include <cassert>
#include <fstream>

Sprite::Sprite(const std::string &filename, const std::string &spriteName, int upscale)
{
    // Clear pixels
    pPixels = nullptr;
    
    // Open file
    std::ifstream file;
    file.open(filename, std::ios::binary);
    assert(file);

    // Read the header info
    file.seekg(2);
    int tgaType = file.get();
    assert(tgaType == 2);

    // Get image dimensions
    file.seekg(9, std::ios::cur);
    int w1 = file.get();
    int w2 = file.get();
    int h1 = file.get();
    int h2 = file.get();

    width  = w1 + w2 * 256;
    height = h1 + h2 * 256;

    // Set sprite name
    this->spriteName = spriteName;

    // Allocate memory for the image
    pPixels = new Color[width*height];

    // Read the image
    file.seekg(2, std::ios::cur);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int b = file.get();
            int g = file.get();
            int r = file.get();
            int a = file.get();

            pPixels[(height - 1 - y) * width + x] = Color(a, r, g, b);
        }
    }

    // Upscale the sprite
    Upscale(upscale);

    // Check if sprite is animated
    int l = filename.length();
    if (l >= 4) {
        if (filename.substr(l-8, 2) == "-f") {
            std::string fcount = filename.substr(l-6, 2);

            frames = str_to_int(fcount);
        }
    }

    // Close file
    file.close();
}

Sprite::Sprite(int width, int height, std::string spriteName, int frames, int centerx, int centery)
    :
    width(width),
    height(height),
    spriteName(spriteName),
    frames(frames),
    centerX(centerx),
    centerY(centery)
{
    pPixels = nullptr;
    pPixels = new Color[width*height];
}

Sprite::Sprite(const Sprite& rhs)
    :
    Sprite(rhs.GetWidth(), rhs.GetHeight(), rhs.GetName(), rhs.GetFrames(), rhs.GetCenterX(), rhs.GetCenterY())
{
    const int nPixels = width * height;
    for (int i = 0; i < nPixels; i++) {
        pPixels[i] = rhs.pPixels[i];
    }
}

Sprite::~Sprite()
{
    delete [] pPixels;
    pPixels = nullptr;
}

Sprite& Sprite::operator=(const Sprite& rhs)
{
    width = rhs.GetWidth();
    height = rhs.GetHeight();
    spriteName = rhs.GetName();
    frames = rhs.GetFrames();
    centerX = rhs.GetCenterX();
    centerY = rhs.GetCenterY();

    delete [] pPixels;
    pPixels = new Color[width*height];

    const int nPixels = width * height;
    for (int i = 0; i < nPixels; i++) {
        pPixels[i] = rhs.pPixels[i];
    }

    return *this;
}

void Sprite::PutPixel(int x, int y, Color c)
{
    assert(x >= 0);
    assert(x < width);
    assert(y >= 0);
    assert(y < height);
    pPixels[y * width + x] = c;
}

void Sprite::PutPixel(int i, Color c)
{
    assert(i >= 0);
    assert(i < width * height);
    pPixels[i] = c;
}

void Sprite::SetCenterX(int x)
{
    centerX = x;
}

void Sprite::SetCenterY(int y)
{
    centerY = y;
}

void Sprite::SetCenterPos(int x, int y)
{
    centerX = x;
    centerY = y;
}

void Sprite::RotateClockwise(int times)
{
    // TRANSFORMATION FORMULA
    //
    //   Initial coords: ( x ; y )
    //   Final coords: ( (OLDHEIGHT|NEWWIDTH) - y - 1 ; x )
    //

    for (int t = 0; t < times; t++) {
        int fwidth = width / frames;
        int fheight = height;

        int newfwidth = fheight;
        int newfheight = fwidth;

        int newcenterx = fheight - centerY - 1;
        int newcentery = centerX;

        // Rotate individual frames
        std::vector<Sprite> newSprites;
        for (int i = 0; i < frames; i++) {
            Sprite ns(newfwidth, newfheight, spriteName, 0, 0, 0);

            for (int x = 0; x < fwidth; x++) {
                for (int y = 0; y < fheight; y++) {
                    // Full sprite coords
                    int sx = x + i * fwidth;
                    int sy = y;

                    ns.PutPixel(height - y - 1, x, pPixels[sy * width + sx]);
                }
            }

            newSprites.push_back(ns);
        }

        // Combine into 1 sprite
        width  = newfwidth * frames;
        height = newfheight;
        centerX = newcenterx;
        centerY = newcentery;

        delete[] pPixels;
        pPixels = new Color[width*height];

        //Sprite combSpr(newfwidth * frames, newfheight, spriteName, frames, newcenterx, newcentery);

        for (int i = 0; i < frames; i++) {

            for (int x = 0; x < newfwidth; x++) {
                for (int y = 0; y < newfheight; y++) {
                    int sx = x + i * newfwidth;
                    int sy = y;

                    PutPixel(sx, sy, newSprites[i].GetPixel(x, y));
                }
            }
        }
    }
}

void Sprite::FlipHorizontal()
{
    int fwidth = width / frames;
    int fheight = height;

    for (int i = 0; i < frames; i++) {
        for (int x = 0; x < fwidth / 2; x++) {
            for (int y = 0; y < fheight; y++) {
                int lindex = y * width +               x  + (i * fwidth);
                int rindex = y * width + (fwidth - 1 - x) + (i * fwidth);

                Color l = pPixels[lindex];
                pPixels[lindex] = pPixels[rindex];
                pPixels[rindex] = l;
            }
        }
    }
}

void Sprite::FlipVertical()
{
    for (int y = 0; y < height / 2; y++) {
        for (int x = 0; x < width; x++) {
            int uindex =               y  * width + x;
            int dindex = (height - 1 - y) * width + x;

            Color u = pPixels[uindex];
            pPixels[uindex] = pPixels[dindex];
            pPixels[dindex] = u;
        }
    }
}

void Sprite::Upscale(int ratio)
{
    // Create pointer to new picture
    Color* newPixels;

    // Set new dimensions
    int newWidth  = width * ratio;
    int newHeight = height * ratio;

    // Allocate memory
    newPixels = new Color[newWidth * newHeight];

    // Go over the original image
    for (int y1 = 0; y1 < height; y1++) {
        for (int x1 = 0; x1 < width; x1++) {
            // Set array position
            int arrayPos = y1 * width + x1;

            // Create dummy variables for simplicity
            int nr = pPixels[arrayPos].GetR();
            int ng = pPixels[arrayPos].GetG();
            int nb = pPixels[arrayPos].GetB();
            int na = pPixels[arrayPos].GetA();

            // Write to the new image
            for (int y2 = y1 * ratio; y2 < (y1 + 1) * ratio; y2++) {
                for (int x2 = x1 * ratio; x2 < (x1 + 1) * ratio; x2++) {
                    // Set array position
                    int newArrayPos = y2 * newWidth + x2;

                    // Write new pixels
                    newPixels[newArrayPos].SetR(nr);
                    newPixels[newArrayPos].SetG(ng);
                    newPixels[newArrayPos].SetB(nb);
                    newPixels[newArrayPos].SetA(na);
                }
            }
        }
    }

    // Clear old image pointer
    pPixels = nullptr;

    // Fill new data
    pPixels = newPixels;

    width  = newWidth;
    height = newHeight;
}

Color Sprite::GetPixel(int id) const
{
    assert(id >= 0);
    assert(id < width * height);
    return pPixels[id];
}

Color Sprite::GetPixel(int x, int y) const
{
    assert(x >= 0);
    assert(x < width);
    assert(y >= 0);
    assert(y < height);
    return pPixels[y * width + x];
}

Color Sprite::GetPixel(int x, int y, float brightness) const
{
    assert(x >= 0);
    assert(x < width);
    assert(y >= 0);
    assert(y < height);

    Color set = pPixels[y * width + x];
    int newR = set.GetR() * brightness;
    int newG = set.GetG() * brightness;
    int newB = set.GetB() * brightness;

    set.SetR(newR);
    set.SetG(newG);
    set.SetB(newB);

    return set;
}

int Sprite::GetWidth() const
{
    return width;
}

int Sprite::GetHeight() const
{
    return height;
}

int Sprite::GetFrames() const
{
    return frames;
}

int Sprite::GetCenterX() const
{
    return centerX;
}

int Sprite::GetCenterY() const
{
    return centerY;
}

std::string Sprite::GetName() const
{
    return spriteName;
}

void Sprite::ApplyTransparency(float t)
{
    for (int i = 0; i < width * height; i++) {
        pPixels[i].SetA(pPixels[i].GetA() * t);
    }
}

Sprite get_sprite_region(Sprite s, RECT r)
{
    // Create new sprite
    Sprite snew(r.right - r.left, r.bottom - r.top, "region", 1, 0, 0);

    // Fill it with a region of the original sprite
    for (int y = r.top; y < r.bottom; y++) {
        for (int x = r.left; x < r.right; x++) {
            int i1 = y             * s.GetWidth() + x;
            int i2 = (y - r.top) * s.GetWidth() + (x - r.left);

            snew.PutPixel(i2, s.GetPixel(i1));
        }
    }

    // Return new sprite
    return snew;
}

Sprite create_rectangle(int w, int h, Color c)
{
    Sprite rectangle(w, h, "rect", 1, w/2, h/2);

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            rectangle.PutPixel(x, y, c);
        }
    }

    return rectangle;
}

Sprite create_circle(int r, Color c, bool C)
{
    int s;
    if (C) s = r * 2 - 1;
    else   s = r * 2;

    Sprite circle(s, s, "circ", 0, 0, 0);

    for (int y = 0; y < s; y++) {
        for (int x = 0; x < s; x++) {
            //if (distance())
        }
    }

    return circle;
}
