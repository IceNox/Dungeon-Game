#include "wtypes.h"
#include "ScreenAnimations.h"

#include <cmath>
#include <sstream>

using namespace std::chrono;

ScreenAnimations::ScreenAnimations()
{
    SAstart = system_clock::now() - milliseconds(250);
}

void ScreenAnimations::set_window_parameters(int w, int h)
{
    windowWidth  = w;
    windowHeight = h;
}

void ScreenAnimations::start_shake(int strength)
{
    duration<float> seed = system_clock::now() - classStart;
    srand(seed.count());

    strength++;
    if (strength > 10) strength = 10;

    int n1 = 10 * strength;
    int n2 = 20 * strength;
    int n3 = 6  * strength;
    int n4 = 10 * strength;
    int n5 = 3  * strength;
    int n6 = 6  * strength;
    int n7 = 1  * strength;
    int n8 = 2  * strength;

    x1 = rand() % (n1 * 2 + 1) - n1;
    y1 = rand() % (n2 / 2 + 1) + n2;

    x2 = rand() % (n3 * 2 + 1) - n3;
    y2 = rand() % (n4 / 2 + 1) - n4;

    x3 = rand() % (n5 * 2 + 1) - n5;
    y3 = rand() % (n6 / 2 + 1) + n6;

    x4 = rand() % (n7 * 2 + 1) - n7;
    y4 = rand() % (n8 / 2 + 1) - n8;

    SAstart = system_clock::now();
}

void ScreenAnimations::apply_shake(int &startX, int &startY)
{
    duration<float> elapsed_seconds = system_clock::now() - SAstart;

    float SAtimeElapsed = elapsed_seconds.count();
    float SAprogress    = SAtimeElapsed / 0.15f;

    if (SAprogress >= 1.0f) {
        return;
    }

    int Xchange;
    int Ychange;

    if (SAprogress <= 0.15f) {
        Xchange = x1 * (1.0f / 0.15f) * SAprogress;
        Ychange = y1 * (1.0f / 0.15f) * SAprogress;
    }
    else if (SAprogress <= 0.4f) {
        int X = x2 - x1;
        int Y = y2 - y1;

        Xchange = X * (1.0f / 0.25f) * (SAprogress - 0.15f);
        Ychange = Y * (1.0f / 0.25f) * (SAprogress - 0.15f);

        Xchange += x1;
        Ychange += y1;
    }
    else if (SAprogress <= 0.6f) {
        int X = x3 - x2;
        int Y = y3 - y2;

        Xchange = X * (1.0f / 0.25f) * (SAprogress - 0.15f);
        Ychange = Y * (1.0f / 0.25f) * (SAprogress - 0.15f);

        Xchange += x2;
        Ychange += y2;
    }
    else if (SAprogress <= 0.8f) {
        int X = x4 - x3;
        int Y = y4 - y3;

        Xchange = X * (1.0f / 0.25f) * (SAprogress - 0.15f);
        Ychange = Y * (1.0f / 0.25f) * (SAprogress - 0.15f);

        Xchange += x3;
        Ychange += y3;
    }
    else {
        int X = -x4;
        int Y = -y4;

        Xchange = X * (1.0f / 0.25f) * (SAprogress - 0.15f);
        Ychange = Y * (1.0f / 0.25f) * (SAprogress - 0.15f);

        Xchange += x4;
        Ychange += y4;
    }

    startX += Xchange;
    startY += Ychange;
}

void ScreenAnimations::start_slow()
{
    SEAstart = system_clock::now();
}

void ScreenAnimations::apply_slow(Color pSysBuffer[])
{
    duration<float> elapsed_seconds = system_clock::now() - SEAstart;

    float SEAtimeElapsed = elapsed_seconds.count();
    float SEAprogress     = sqrt(SEAtimeElapsed / 0.75f);

    if (SEAprogress >= 1.0f) return;

    int startY = -400 + round((windowHeight + 400) * SEAprogress);

    for (int y = startY; y < startY + 400; y++) {

        if (y >= windowHeight) return;

        float b;

        if        (y - startY < 80 ) {
            b = 0.01f;
        }
        else if (y - startY < 160) {
            b = 0.02f;
        }
        else if (y - startY < 240) {
            b = 0.03f;
        }
        else if (y - startY < 320) {
            b = 0.04f;
        }
        else if (y - startY < 400) {
            b = 0.05f;
        }

        for (int x = 0; x < windowWidth; x++) {
            const int index = y * windowWidth + x;

            /*
            if (startY >= 400) {
                std::wostringstream ss;
                ss << index;
                ss << L"\n";
                OutputDebugString(ss.str().c_str());
            }
            */

            if (index < windowWidth * windowHeight && index >= 0) {
                Color p = pSysBuffer[index];

                p.SetR((float)p.GetR() * (1.0f - b));
                p.SetG((float)p.GetG() + (255 - p.GetG()) * b);
                p.SetB((float)p.GetB() + (255 - p.GetB()) * b);

                pSysBuffer[index] = p;
            }
        }
    }
}

void ScreenAnimations::start_death_effect()
{
    DEstart = system_clock::now();
}

void ScreenAnimations::apply_death_effect(Color pSysBuffer[])
{
    duration<float> elapsed_seconds = system_clock::now() - DEstart;

    float DEtimeElapsed = elapsed_seconds.count();
    float DEprogress    = sqrt(DEtimeElapsed / 0.5f);

    if (DEprogress >= 1.0f) return;

    Color n(255, 0, 0);
    n.SetR(255 - round(255.0f * DEprogress));

    for (int i = 0; i < windowHeight * windowWidth; i++) {
            pSysBuffer[i] = n;
    }
}

void ScreenAnimations::start_end_effect()
{
    EEstart = system_clock::now();
}

void ScreenAnimations::apply_end_effect(Color pSysBuffer[])
{
    duration<float> elapsed_seconds = system_clock::now() - EEstart;

    float EEtimeElapsed = elapsed_seconds.count();
    float EEprogress    = sqrt(EEtimeElapsed);

    if (EEprogress >= 1.0f) EEprogress = 1.0f;

    //Color n(255, 0, 0);
    //n.SetR(255 - round(255.0f * EEprogress));

    for (int i = 0; i < windowHeight * windowWidth; i++) {
        Color c = pSysBuffer[i];

        c.SetR(c.GetR() * (1.0f - sqrt(EEprogress)));
        c.SetG(c.GetG() * (1.0f - sqrt(EEprogress)));
        c.SetB(c.GetB() * (1.0f - sqrt(EEprogress)));

        pSysBuffer[i] = c;
    }
}