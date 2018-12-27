#include "Animation.h"

#include "GlobalData.h"

#include <cmath>
#include <sstream>

Animation::Animation
(
    Pos2D cPos,
    bool ground,
    int spri,
    int holdtime
)   :
    cPos(cPos),
    ground(ground),
    spriteIndex(spri),
    frameHoldTime(holdtime)
{
    spriteOffset = { -sprites[spri].GetCenterX(), -sprites[spri].GetCenterY() };
    spriteWidth = sprites[spri].GetWidth() / sprites[spri].GetFrames();
    spriteHeight = sprites[spri].GetHeight();
    frameCount = sprites[spri].GetFrames();

    currentFrame = 0;
    frameHoldTime = holdtime;
    spriteRegion = { 0, 0, spriteWidth, spriteHeight };
    sPos = cPos + spriteOffset;

    creationTime = maintime::now();

    finished = false;
}

Animation::Animation(const LevelMessage &msg, std::string &result)
{
    // Get arguments
    std::vector<std::string> args;
    args.reserve(10);

    split_str(msg.message, args);

    if (args.size() < 3) {
        result = "error: not enough arguments";
        return;
    }

    /// Evaluate mandatory arguments
    // Position
    cPos = { str_to_int(args[0]), str_to_int(args[1]) };

    // Sprite
    spriteIndex = str_to_int(args[2]);

    /// Evaluate additional arguments
    if (msg.argKeys.size() > 0) {
        // Frame hold time
        int ht = msg.int_at("fholdtime");
        if (ht != -1)
            frameHoldTime = ht;
        else
            frameHoldTime = 150;

        // Ground
        ground = msg.int_at("ground");
    }

    spriteOffset = { -sprites[spriteIndex].GetCenterX(), -sprites[spriteIndex].GetCenterY() };
    spriteWidth = sprites[spriteIndex].GetWidth() / sprites[spriteIndex].GetFrames();
    spriteHeight = sprites[spriteIndex].GetHeight();
    frameCount = sprites[spriteIndex].GetFrames();

    currentFrame = 0;
    spriteRegion = { 0, 0, spriteWidth, spriteHeight };
    sPos = cPos + spriteOffset;

    creationTime = maintime::now();

    finished = false;
}

void Animation::update_animation()
{
    currentFrame = floor((maintime::now() - creationTime).get_duration(MILLISECONDS) / frameHoldTime);
    if (currentFrame >= frameCount) finished = true;
    else {
        spriteRegion.left  =  currentFrame      * spriteWidth;
        spriteRegion.right = (currentFrame + 1) * spriteWidth;
    }
}
