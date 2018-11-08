#pragma once

#include "Constants.h"

#include "Position.h"
#include "Sprite.h"

#include "Particle.h"
#include "Animation.h"
#include "Entity.h"

#include <string>
#include <vector>

class LevelMessage
{
	std::string msg;
	std::string src;
public:
	LevelMessage(std::string msg, std::string src = "unspecified") : msg(msg), src(src) {};

	std::string handle_msg
	(
		std::vector<Particle> &particles,
		std::vector<Animation> &animations,
		std::vector<Entity> &entities
	)
	{

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
		int			damage,
		int			heal,
		int			hitboxRadius,
		bool		cwp,
		bool		cwa,
		bool		animated,
		SpriteName	spriteName,
		int			sW,
		int			sH,
		int			frameCount,
		int			frameHoldTime
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
		int	goldAmount
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
		if		(type == 0) {
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