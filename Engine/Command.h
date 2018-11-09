#pragma once

#include "Sprite.h"

#include <string>

class Command
{
public:
    /// What to do
    bool spawn              = false;
    bool damageInRadius   = false;
    bool healInRadius      = false;
    bool createEntity      = false;
    bool createParticle      = false;
    bool showVisualEffect = false;

    /// Spawn variables
    int mobX;
    int mobY;
    std::string mobName;
    std::string mobType;
    bool mobAnimated;

    /// Damage/Heal in radius variables
    std::string effectType;

    int centerX;
    int centerY;
    int effectRadius;
    int effectAmount;

    /// Create entity
    std::string entityType;

    int entitygX;
    int entitygY;
    // Regular entity
    int entitytX;
    int entitytY;
    int entityDamage;
    int entityHeal;
    int entityHitboxRadius;
    int entitySpriteoX;
    int entitySpriteoY;
    bool entityCollidesWithPlayer;
    bool entityCollidesWithAI;
    float entityvX;
    float entityvY;
    int entityRangeX;
    int entityRangeY;
    bool entityAnimated;
    std::string entityBaseSpriteName;
    int entitySpriteW;
    int entitySpriteH;
    int entityFrameCount;
    float entityFrameHoldTime;
    // Gold entity
    int goldAmount;
    // Item entity
    std::string itemName;
    std::string itemSpriteName;

    /// Create particle
    std::string particleType;

    int particlecX;
    int particlecY;
    // Regular particle
    Sprite particleSprite;
    int particlevX;
    int particlevY;
    int particleaX;
    int particleaY;
    float particleExistenceLength;
    float particleFadeLength;
    // Text particle
    std::string particleText;

    /// Show visual effect
    int visualEffectsX;
    int visualEffectsY;
    bool visualEffectGround;
    std::string visualEffectName;
    int visualEffectSpriteW;
    int visualEffectSpriteH;
    float visualEffectHoldTime;
    int visualEffectFrameCount;
    float visualEffectTransparency;

    Command // Spawn constructor
    (
        int mobx,
        int moby,
        std::string mobname,
        std::string mobtype,
        bool animated
    )    :
        mobX(mobx),
        mobY(moby),
        mobName(mobname),
        mobType(mobtype),
        mobAnimated(animated),
        particleSprite(create_rectangle(1, 1, Color(0, 0, 0)))
    {
        spawn = true;
    }

    Command // Damage/Heal in radius constructor
    (
        std::string eType,
        int centerX,
        int centerY,
        int radius,
        int amount
    ) :
        effectType(eType),
        centerX(centerX),
        centerY(centerY),
        effectRadius(radius),
        effectAmount(amount),
        particleSprite(create_rectangle(1, 1, Color(0, 0, 0)))
    {
        if        (eType == "damage") damageInRadius = true;
        else if (eType == "heal")    healInRadius   = true;
    }

    Command // Create regular entity constructor
    (
        int egX,
        int egY,
        int etX,
        int etY,
        int eDamage,
        int eHeal,
        int eHitboxRadius,
        int eSpriteoX,
        int eSpriteoY,
        bool cwp,
        bool cwa,
        int evX,
        int evY,
        int erX,
        int erY,
        bool animated,
        std::string eBaseSpriteName,
        int eSpriteW,
        int eSpriteH,
        int eFrameCount,
        float eFrameHoldTime
    )    :
        entitygX(egX),
        entitygY(egY),
        entitytX(etX),
        entitytY(etY),
        entityDamage(eDamage),
        entityHeal(eHeal),
        entityHitboxRadius(eHitboxRadius),
        entitySpriteoX(eSpriteoX),
        entitySpriteoY(eSpriteoY),
        entityCollidesWithPlayer(cwp),
        entityCollidesWithAI(cwa),
        entityvX(evX),
        entityvY(evY),
        entityRangeX(erX),
        entityRangeY(erY),
        entityAnimated(animated),
        entityBaseSpriteName(eBaseSpriteName),
        entitySpriteW(eSpriteW),
        entitySpriteH(eSpriteH),
        entityFrameCount(eFrameCount),
        entityFrameHoldTime(eFrameHoldTime),
        particleSprite(create_rectangle(1, 1, Color(0, 0, 0)))
    {
        createEntity = true;
        entityType = "regular";
    }

    Command // Create gold entity constructor
    (
        int egX,
        int egY,
        int amount
    )    :
        entitygX(egX),
        entitygY(egY),
        goldAmount(amount),
        particleSprite(create_rectangle(1, 1, Color(0, 0, 0)))
    {
        createEntity = true;
        entityType     = "gold";
    }

    Command // Create item entity constructor
    (
        int egX,
        int egY,
        std::string itemName,
        std::string itemSpriteName
    )    :
        entitygX(egX),
        entitygY(egY),
        itemName(itemName),
        itemSpriteName(itemSpriteName),
        particleSprite(create_rectangle(1, 1, Color(0, 0, 0)))
    {
        createEntity = true;
        entityType = "gold";
    }

    Command // Create regular particle constructor
    (
        int pcX,
        int pcY,
        Sprite pSprite,
        int pvX,
        int pvY,
        int paX,
        int paY,
        float pExistenceLength,
        float pFadeLength
    )    :
        particlecX(pcX),
        particlecY(pcY),
        particleSprite(pSprite),
        particlevX(pvX),
        particlevY(pvY),
        particleaX(paX),
        particleaY(paY),
        particleExistenceLength(pExistenceLength),
        particleFadeLength(pFadeLength)
    {
        createParticle = true;
        particleType = "regular";
    }

    Command // Create text particle constructor
    (
        int pcX,
        int pcY,
        std::string pText
    )    :
        particlecX(pcX),
        particlecY(pcY),
        particleText(pText),
        particleSprite(create_rectangle(1, 1, Color(0, 0, 0)))
    {
        createParticle = true;
        particleType = "text";
    }

    Command // Show visual effect constructor
    (
        int vfxcX,
        int vfxcY,
        bool vfxground,
        std::string vfxName,
        int vfxSpriteW,
        int vfxSpriteH,
        float vfxFrameHoldTime,
        int vfxFrameCount,
        float vfxTransparency
    )   :
        visualEffectsX(vfxcX),
        visualEffectsY(vfxcY),
        visualEffectGround(vfxground),
        visualEffectName(vfxName),
        visualEffectSpriteW(vfxSpriteW),
        visualEffectSpriteH(vfxSpriteH),
        visualEffectHoldTime(vfxFrameHoldTime),
        visualEffectFrameCount(vfxFrameCount),
        visualEffectTransparency(vfxTransparency),
        particleSprite(create_rectangle(1, 1, Color(0, 0, 0)))
    {
        showVisualEffect = true;
    }
};