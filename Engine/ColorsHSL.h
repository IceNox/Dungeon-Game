#pragma once
#undef min
#undef max

#include <algorithm>
#include <cmath>

#include "Colors.h"

// Constants
const double DEPTH = 255.0f;

class ColorHSL
{
private:
    double _hue;
    double _saturation;
    double _luminance;

public:
    constexpr ColorHSL()
        :
        _hue(),
        _saturation(),
        _luminance()
    {}

    constexpr ColorHSL(const ColorHSL& col)
        :
        _hue(col._hue),
        _saturation(col._saturation),
        _luminance(col._luminance)
    {}

    constexpr ColorHSL(unsigned int hue, unsigned int saturation, unsigned int luminance)
        :
        _hue(hue),
        _saturation(saturation),
        _luminance(luminance)
    {}

    constexpr ColorHSL(double hue, double saturation, double luminance)
        :
        _hue(hue),
        _saturation(saturation),
        _luminance(luminance)
    {}

    ColorHSL& operator= (ColorHSL col) {
        _hue = col._hue;
        _saturation = col._saturation;
        _luminance = col._luminance;
        return *this;
    }

    bool operator== (const ColorHSL& col) const {
        return (_hue == col._hue && _saturation == col._saturation && _luminance == col._luminance);
    }

    bool operator!= (const ColorHSL& col) const {
        return !(*this == col);
    }

    constexpr double GetH() const {
        return _hue;
    }

    constexpr double GetS() const {
        return _saturation;
    }

    constexpr double GetL() const {
        return _luminance;
    }

    void SetH(double hue) {
        _hue = hue;
    }

    void SetS(double saturation) {
        _saturation = saturation;
    }

    void SetL(double luminance) {
        _luminance = luminance;
    }

    static ColorHSL RGBtoHSL(const Color& col) {
        double R = col.GetR() / DEPTH;
        double G = col.GetG() / DEPTH;
        double B = col.GetB() / DEPTH;

        double min = std::min(R, std::min(G, B));
        double max = std::max(R, std::max(G, B));

        double luminance = (min + max) / 2;
        double saturation = 0.0f;
        double hue = 0.0f;

        if (min == max) {
            return ColorHSL(hue, saturation, luminance);
        }
        else {
            if (luminance < 0.5f) {
                saturation = (max - min) / (max + min);
            }
            else {
                saturation = (max - min) / (2.0f - max - min);
            }
        }

        if (R == max) {
            hue = (G - B) / (max - min);
        }
        else if (G == max) {
            hue = (B - R) / (max - min) + 2.0f;
        }
        else if (B == max) {
            hue = (R - G) / (max - min) + 4.0f;
        }

        hue *= 60.0f;
        if (hue < 0.0f) {
            hue += 360.0f;
        }

        return ColorHSL(hue, saturation, luminance);
    }

    static Color HSLtoRGB(const ColorHSL& col) {
        double val_1 = 0.0f;
        double val_2 = 0.0f;
        double val_3 = 0.0f;

        if (col._luminance < 0.5f) {
            val_1 = col._luminance * (1.0f + col._saturation);
        }
        else {
            val_1 = col._luminance + col._saturation - col._luminance * col._saturation;
        }

        val_2 = 2.0f * col._luminance - val_1;
        val_3 = col._hue / 360.0f;

        double R = std::fmod(val_3 + (1.0f / 3.0f) + 1.0f, 1.0f);
        double G = std::fmod(val_3 + 1.0f, 1.0f);
        double B = std::fmod(val_3 - (1.0f / 3.0f) + 1.0f, 1.0f);

        if (6.0f * R < 1.0f) {
            R = val_2 + (val_1 - val_2) * 6 * R;
        }
        else if (2.0f * R < 1.0f) {
            R = val_1;
        }
        else if (3.0f * R < 2.0f) {
            R = val_2 + (val_1 - val_2) * (2.0f / 3.0f - R) * 6.0f;
        }
        else {
            R = val_2;
        }

        if (6.0f * G < 1.0f) {
            G = val_2 + (val_1 - val_2) * 6 * G;
        }
        else if (2.0f * G < 1.0f) {
            G = val_1;
        }
        else if (3.0f * G < 2.0f) {
            G = val_2 + (val_1 - val_2) * (2.0f / 3.0f - G) * 6.0f;
        }
        else {
            G = val_2;
        }

        if (6.0f * B < 1.0f) {
            B = val_2 + (val_1 - val_2) * 6 * B;
        }
        else if (2.0f * B < 1.0f) {
            B = val_1;
        }
        else if (3.0f * B < 2.0f) {
            B = val_2 + (val_1 - val_2) * (2.0f / 3.0f - B) * 6.0f;
        }
        else {
            B = val_2;
        }

        return Color(
            static_cast<unsigned int>(round(R * DEPTH)),
            static_cast<unsigned int>(round(G * DEPTH)),
            static_cast<unsigned int>(round(B * DEPTH))
        );
    }
};
