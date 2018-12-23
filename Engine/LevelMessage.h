#pragma once

#include "Constants.h"

#include "Functions.h"
#include "Navigation.h"

#include <string>
#include <vector>
#include <exception>

namespace lmsg {
    class ReturnVal
    {
        int valI;
        std::string valS;

        int type;
        bool ok;

    public:
        bool valid()
        {
            return ok;
        }

        int valueI()
        {
            return valI;
        }

        std::string valueS()
        {
            return valS;
        }
    };
}

class LevelMessage
{
public:
    std::string source;
    std::string message;
    std::vector<std::string> argKeys;
    std::vector<std::string> argValsStr;
    std::vector<    int    > argValsInt;

    LevelMessage()
    {
        source = "";
        message = "";
    }
    LevelMessage(std::string msg, std::string src = "???") : source(src)
    {
        std::vector<std::string> args;
        split_str(msg, args);

        // Split additional arguments
        for (unsigned i = 0; i < args.size(); i++) {
            if (args[i][0] == '[') {
                // Remove brackets
                args[i].pop_back();
                args[i].erase(args[i].begin());

                // Remove '=' and split the values
                for (unsigned j = 0; j < args[i].length(); j++) {
                    if (args[i][j] == '=') {
                        args[i][j] = ' ';
                        break;
                    }
                }

                std::vector<std::string> kvpair;
                split_str(args[i], kvpair);

                // Get key and value
                if (kvpair.size() == 2) {
                    if (kvpair[1] == "true" ) kvpair[1] = "1";
                    if (kvpair[1] == "false") kvpair[1] = "0";

                    argKeys.push_back(kvpair[0]);
                    argValsStr.push_back(kvpair[1]);
                    argValsInt.push_back(str_to_int(kvpair[1]));

                    args.erase(args.begin() + i);
                    i--;
                }
                else {
                    continue;
                }
            }
        }

        // Recombine message
        message = "";
        for (unsigned i = 0; i < args.size(); i++) {
            message += args[i];

            if (i < args.size() - 1) {
                message += ' ';
            }
        }
    }
    LevelMessage(const LevelMessage &msg)
    {
        source = msg.source;
        message = msg.message;
        argKeys = msg.argKeys;
        argValsStr = msg.argValsStr;
        argValsInt = msg.argValsInt;
    }

    int index_at(const std::string &key) const
    {
        for (unsigned i = 0; i < argKeys.size(); i++) {
            if (key == argKeys[i]) {
                return i;
            }
        }

        throw std::out_of_range("Key doesn't exist");
        return -1;
    }

    int int_at(const std::string &key) const
    {
        for (unsigned i = 0; i < argKeys.size(); i++) {
            if (key == argKeys[i]) {
                return argValsInt[i];
            }
        }

        throw std::out_of_range("Key doesn't exist");
        return -1;
    }

    std::string str_at(const std::string &key) const
    {
        for (unsigned i = 0; i < argKeys.size(); i++) {
            if (key == argKeys[i]) {
                return argValsStr[i];
            }
        }

        throw std::out_of_range("Key doesn't exist");
        return "";
    }
};

/*
class LevelMessage
{
public:
    virtual ~LevelMessage() = default;

    virtual void handle_msg
    (
        std::vector<Particle> &particles,
        std::vector<Animation> &animations,
        std::vector<Entity> &entities
    )
    {

    }
};

class MsgCreateParticle : public LevelMessage
{
    int type; // 0 - regular particle; 1 - text

    Pos2D cPos;
    Pos2D vVec;
    Pos2D aVec;
    Sprite s;
    int eLength;
    int fLength;
    std::string text;
public:
    MsgCreateParticle
    (
        Pos2D cPos,
        Pos2D vVec,
        Pos2D aVec,
        Sprite s,
        int eLength,
        int fLength
    ) :
        cPos(cPos),
        vVec(vVec),
        aVec(aVec),
        s(s),
        eLength(eLength),
        fLength(fLength)
    {
        type = 0;
    }
    
    MsgCreateParticle
    (
        Pos2D cPos,
        std::string text
    ) :
        cPos(cPos),
        text(text)
    {
        type = 1;
    }

    virtual void handle_msg
    (
        std::vector<Particle> &particles,
        std::vector<Animation> &animations,
        std::vector<Entity> &entities
    )
    {
        if (type == 0) {
            particles.push_back
            (
                Particle
                (
                    cPos,
                    vVec,
                    aVec,
                    s,
                    eLength,
                    fLength
                )
            );
        }
        else if (type == 1) {
            particles.push_back
            (
                Particle
                (
                    cPos,
                    text
                )
            );
        }
    }
};

class MsgShowAnimation : public LevelMessage
{
    Pos2D cPos;
    bool ground;
    SpriteName name;
    int spriteW;
    int spriteH;
    int holdTime;
    int frameCount;
public:
    MsgShowAnimation
    (
        Pos2D cPos,
        bool g,
        SpriteName name,
        int sw,
        int sh,
        int ht,
        int fc
    ) :
        cPos(cPos),
        ground(g),
        name(name),
        spriteW(sw),
        spriteH(sh),
        holdTime(ht),
        frameCount(fc)
    {}

    virtual void handle_msg
    (
        std::vector<Particle> &particles,
        std::vector<Animation> &animations,
        std::vector<Entity> &entities
    )
    {
        animations.push_back
        (
            Animation
            (
                cPos,
                ground,
                name,
                spriteW,
                spriteH,
                holdTime,
                frameCount
            )
        );
    }
};

class MsgStartScreenEffect : public LevelMessage
{

};

class MsgStartTileEffect : public LevelMessage
{

};

class MsgCreateEntity : public LevelMessage
{
    int type; // 0 - regular entity; 1 - gold entity; 2 - item entity

    bool collidesWithPlayer;
    bool collidesWithAI;

    Pos2D gPos;
    Pos2D cPos;
    Pos2D vVec;
    Pos2D rangeVec;

    Pos2D sOffset;

    bool item;
    bool gold;

    std::string itemName;
    int goldAmount;
    int damage;
    int heal;

    int hitboxRadius;

    bool animated;

    int sHeight;
    int sWidth;

    int frameCount;
    int frameHoldTime;
    int currentFrame;

    SpriteName spriteName;
public:
    MsgCreateEntity
    (
        Pos2D cPos,
        Pos2D vVec,
        Pos2D sOffset,
        Pos2D rangeVec,
        int            damage,
        int            heal,
        int            hitboxRadius,
        bool        cwp,
        bool        cwa,
        bool        animated,
        SpriteName    spriteName,
        int            sW,
        int            sH,
        int            frameCount,
        int            frameHoldTime
    ) :
        cPos(cPos),
        vVec(vVec),
        sOffset(sOffset),
        rangeVec(rangeVec),
        damage(damage),
        heal(heal),
        hitboxRadius(hitboxRadius),
        collidesWithPlayer(cwp),
        collidesWithAI(cwa),
        animated(animated),
        spriteName(spriteName),
        sWidth(sW),
        sHeight(sH),
        frameCount(frameCount),
        frameHoldTime(frameHoldTime)
    {
        type = 0;
    }

    MsgCreateEntity
    (
        Pos2D gPos,
        int    goldAmount
    ) :
        gPos(gPos),
        goldAmount(goldAmount)
    {
        type = 1;
    }

    MsgCreateEntity
    (
        Pos2D gPos,
        SpriteName itemName
    ) :
        gPos(gPos),
        spriteName(itemName)
    {
        type = 2;
    }

    virtual void handle_msg
    (
        std::vector<Particle> &particles,
        std::vector<Animation> &animations,
        std::vector<Entity> &entities
    )
    {
        if        (type == 0) {
            entities.push_back
            (
                Entity
                (
                    cPos,
                    vVec,
                    sOffset,
                    rangeVec,
                    damage,
                    heal,
                    hitboxRadius,
                    collidesWithPlayer,
                    collidesWithAI,
                    animated,
                    spriteName,
                    sWidth,
                    sHeight,
                    frameCount,
                    frameHoldTime
                )
            );
        }
        else if (type == 1) {
            entities.push_back
            (
                Entity
                (
                    gPos,
                    goldAmount
                )
            );
        }
        else if (type == 2) {
            entities.push_back
            (
                Entity
                (
                    gPos,
                    spriteName
                )
            );
        }
    }
};

class MsgSpawnMob : public LevelMessage
{
    Pos2D gPos;
    MobName name;
    MobType type;
public:
    MsgSpawnMob
    (
        Pos2D gPos,
        MobName name,
        MobType type
    ) :
        gPos(gPos),
        name(name),
        type(type)
    {}

    virtual void handle_msg
    (
        std::vector<Particle> &particles,
        std::vector<Animation> &animations,
        std::vector<Entity> &entities
    )
    {

    }
};

class MsgDamageTile : public LevelMessage
{
    Pos2D gPos;
};

class MsgHealTile : public LevelMessage
{

};
*/
