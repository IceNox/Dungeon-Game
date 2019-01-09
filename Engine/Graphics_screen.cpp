#include "Graphics.h"

Sprite Graphics::CaptureScreenshot()
{
    Sprite ss(ScreenWidth, ScreenHeight, "ss", 1, ScreenWidth / 2, ScreenHeight / 2);

    for (int i = 0; i < ScreenWidth * ScreenHeight; i++) {
        ss.PutPixel(i, GetPixel(i));
    }

    return ss;
}

void Graphics::SetLuminance(double luminance)
{
    SetLuminance(luminance, { 0, 0, ScreenWidth, ScreenHeight });
}

void Graphics::SetLuminance(double luminance, RECT r)
{
    return;
}

void Graphics::ChangeLuminance(float factor)
{
    ChangeLuminance(factor, { 0, 0, ScreenWidth, ScreenHeight });
}

void Graphics::ChangeLuminance(float factor, RECT r)
{
    if (factor < 0.0f) factor = 0.0f;
    if (factor > 1.0f) factor = 1.0f;

    for (int y = r.top; y < r.bottom; y++) {
        for (int x = r.left; x < r.right; x++) {
            int index = y * ScreenWidth + x;

            Color    RGBc = pSysBuffer[index];
            ColorHSL HSLc = ColorHSL::RGBtoHSL(RGBc);
            HSLc.SetL(HSLc.GetL() * factor);
            RGBc = ColorHSL::HSLtoRGB(HSLc);
            pSysBuffer[index] = RGBc;
        }
    }
}

/*
void Graphics::ChangeLuminance(float factor, RECT r)
{
    if (factor < 0.0f) factor = 0.0f;
    if (factor > 1.0f) factor = 1.0f;

    for (int y = r.top; y < r.bottom; y++) {
        for (int x = r.left; x < r.right; x++) {
            int index = y * ScreenWidth + x;
            
            Color c = pSysBuffer[index];

            c.SetR(c.GetR() * factor);
            c.SetG(c.GetG() * factor);
            c.SetB(c.GetB() * factor);

            pSysBuffer[index] = c;
        }
    }
}
*/
