#include "Particle.h"

#include "GlobalData.h"

#include <cmath>

Particle::Particle
(
    Pos2D cPos,
    Pos2D vVec,
    Pos2D aVec,
    int spri,
    int eLength,
    int fLength,
    bool fading
) :
    cPos(cPos),
    vVec(vVec),
    aVec(aVec),
    spriteIndex(spri),
    existenceLength(eLength),
    fadeLength(fLength),
    fading(fading),
    dtype(PARTICLE_SPRITE)
{
    cPos_hp.x = cPos.x * 1000;
    cPos_hp.y = cPos.y * 1000;

    vVec_hp.x = vVec.x * 1000;
    vVec_hp.y = vVec.y * 1000;

    spriteWidth = sprites[spri].GetWidth();
    spriteHeight = sprites[spri].GetHeight();
    spriteOffset = { -sprites[spri].GetCenterX(), -sprites[spri].GetCenterY() };
    sPos = cPos + spriteOffset;

    creationTime = maintime::now();
    lastUpdated = maintime::now();

    transparency = 1.0f;

    finished = false;
}

Particle::Particle
(
    Pos2D cPos,
    Pos2D vVec,
    Pos2D aVec,
    Sprite s,
    int eLength,
    int fLength,
    bool fading
) :
    cPos(cPos),
    vVec(vVec),
    aVec(aVec),
    srect(s),
    existenceLength(eLength),
    fadeLength(fLength),
    fading(fading),
    dtype(PARTICLE_RECTANGLE)
{
    cPos_hp.x = cPos.x * 1000;
    cPos_hp.y = cPos.y * 1000;

    vVec_hp.x = vVec.x * 1000;
    vVec_hp.y = vVec.y * 1000;

    spriteWidth = s.GetWidth();
    spriteHeight = s.GetHeight();
    spriteOffset = { -s.GetCenterX(), -s.GetCenterY() };
    sPos = cPos + spriteOffset;

    creationTime = maintime::now();
    lastUpdated  = maintime::now();

    transparency = 1.0f;

    finished = false;
}

Particle::Particle
(
    Pos2D cPos,
    std::string text,
    Color c,
    bool fading
) :
    cPos(cPos),
    vVec(Pos2D(0, -32)),
    aVec(Pos2D(0,  32)),
    text(text),
    rtColor(c),
    existenceLength(1000),
    fadeLength(1000),
    fading(fading),
    dtype(PARTICLE_TEXT)
{
    cPos_hp.x = cPos.x * 1000;
    cPos_hp.y = cPos.y * 1000;

    vVec_hp.x = vVec.x * 1000;
    vVec_hp.y = vVec.y * 1000;

    creationTime = maintime::now();
    lastUpdated  = maintime::now();

    transparency = 1.0f;

    finished = false;
}

Particle::Particle(const LevelMessage &msg, std::string &result)
{
    // Get arguments
    std::vector<std::string> args;
    args.reserve(10);

    split_str(msg.message, args);

    /// Evaluate mandatory arguments
    // Position
    cPos = { str_to_int(args[0]), str_to_int(args[1]) };

    /// Evaluate additional arguments
    if (msg.argKeys.size() > 0) {
        // Velocity
        std::string vv = msg.str_at("velocity");
        if (vv.length() > 0) {
            std::vector<std::string> vals;
            split_str(vv, vals, ';');

            if (vals.size() == 2) {
                vVec = { str_to_int(vals[0]), str_to_int(vals[1]) };
            }
            else {
                vVec = { 0, 0 };
            }
        }
        else {
            vVec = { 0, 0 };
        }

        // Acceleration
        std::string av = msg.str_at("acceleration");
        if (av.length() > 0) {
            std::vector<std::string> vals;
            split_str(av, vals, ';');

            if (vals.size() == 2) {
                aVec = { str_to_int(vals[0]), str_to_int(vals[1]) };
            }
            else {
                aVec = { 0, 0 };
            }
        }
        else {
            aVec = { 0, 0 };
        }

        // Existence/Fade duration
        existenceLength = msg.int_at("elength");
        fadeLength = msg.int_at("flength");

        int f = msg.int_at("fading");
        fading = (f == -1) ? true : f;

        // Color
        std::string c = msg.str_at("color");
        if (c.length() > 0) {
            std::vector<std::string> vals;
            split_str(c, vals, ';');

            if (vals.size() == 3) {
                rtColor = { 255, static_cast<unsigned char>(str_to_int(vals[0])), static_cast<unsigned char>(str_to_int(vals[1])), static_cast<unsigned char>(str_to_int(vals[2])) };
            }
            else {
                rtColor = { 255, 255, 255, 255 };
            }
        }
        else {
            rtColor = { 255, 255, 255, 255 };
        }

        // Sprite
        spriteIndex = msg.int_at("sprite");
        if (spriteIndex >= 0 && spriteIndex < sprites.size()) {
            dtype = PARTICLE_SPRITE;

            spriteWidth = sprites[spriteIndex].GetWidth();
            spriteHeight = sprites[spriteIndex].GetHeight();
            spriteOffset = { -sprites[spriteIndex].GetCenterX(), -sprites[spriteIndex].GetCenterY() };
            sPos = cPos + spriteOffset;
        }

        // Rectangle
        std::string rectI = msg.str_at("rect");
        if (rectI.length() > 0) {
            std::vector<std::string> vals;
            split_str(rectI, vals, ';');

            if (vals.size() == 2) {
                srect = create_rectangle(str_to_int(vals[0]), str_to_int(vals[1]), rtColor);
                dtype = PARTICLE_RECTANGLE;

                spriteWidth = srect.GetWidth();
                spriteHeight = srect.GetHeight();
                spriteOffset = { -srect.GetCenterX(), -srect.GetCenterY() };
                sPos = cPos + spriteOffset;
            }
        }

        // Text
        std::string ptext = msg.str_at("text");
        if (ptext.length() > 0) {
            for (unsigned i = 0; i < ptext.length(); i++) {
                if (ptext[i] == ';') ptext[i] = ' ';
            }

            text = ptext;
            dtype = PARTICLE_TEXT;
        }
    }

    cPos_hp.x = cPos.x * 1000;
    cPos_hp.y = cPos.y * 1000;

    vVec_hp.x = vVec.x * 1000;
    vVec_hp.y = vVec.y * 1000;

    creationTime = maintime::now();
    lastUpdated = maintime::now();

    transparency = 1.0f;

    finished = false;
}

void Particle::update_particle()
{
    if (finished) return;
    if (!fading) {
        finished = true;
        return;
    }

    // Position calculation
    long int usElapsed = (maintime::now() - lastUpdated).get_duration();

    vVec_hp.x += aVec.x * usElapsed / 1000.f;
    vVec_hp.y += aVec.y * usElapsed / 1000.f;

    cPos_hp.x += round(vVec_hp.x * (usElapsed / 1000000.0f));
    cPos_hp.y += round(vVec_hp.y * (usElapsed / 1000000.0f));

    cPos.x = round(cPos_hp.x / 1000.0f);
    cPos.y = round(cPos_hp.y / 1000.0f);

    sPos = cPos + spriteOffset;
    
    // Update time
    lastUpdated = maintime::now();

    // Fade calculation
    long int msElapsed = (maintime::now() - creationTime).get_duration(MILLISECONDS);

    if (msElapsed > existenceLength) {
        transparency = 1.0f;
        finished     = true;
    }
    if (msElapsed < existenceLength - fadeLength) {
        transparency = 1.0f;
        return;
    }

    transparency = 1.0f - (msElapsed - existenceLength + fadeLength) / (float)fadeLength;
    transparency *= transparency;
}
