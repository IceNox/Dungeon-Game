#include "Graphics.h"

Sprite Graphics::CaptureScreenshot()
{
	Sprite ss(ScreenWidth, ScreenHeight, "ss");

	for (int i = 0; i < ScreenWidth * ScreenHeight; i++) {
		ss.PutPixel(i, GetPixel(i));
	}

	return ss;
}

void Graphics::ChangeBrightness(float brightness)
{
	ChangeBrightness(brightness, { 0, 0, ScreenWidth, ScreenHeight });
}

void Graphics::ChangeBrightness(float brightness, RECT r)
{
	if (brightness < 0.0f) brightness = 0.0f;
	if (brightness > 1.0f) brightness = 1.0f;

	for (int y = r.top; y < r.bottom; y++) {
		for (int x = r.left; x < r.right; x++) {
			int index = y * ScreenWidth + x;
			
			Color c = pSysBuffer[index];

			c.SetR(c.GetR() * brightness);
			c.SetG(c.GetG() * brightness);
			c.SetB(c.GetB() * brightness);

			pSysBuffer[index] = c;
		}
	}
}