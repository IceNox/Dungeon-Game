#pragma once

#include "Navigation.h"
#include "Constants.h"

#include <string>
#include <vector>

class LevelData
{
public:
    LevelData() {}
    LevelData(std::string levelFileName);

    // Main data
    std::string levelName;

    int width;
    int height;

    Pos2D playerStartPos;

    // Portals
    int portalCount;
    std::vector<Pos2D> portalPos;
    std::vector<Pos2D> portalExitPos;
    std::vector<int> portalNewLevel;

    // Walls
    int wallCount;
    std::vector<Pos2D> wallGridPos;
    std::vector<TileVariant> wallVariant;
    std::vector<bool> wallActivatable;
    std::vector<bool> wallToggleable;
    std::vector<bool> wallActive;

    /*
    int waterCount;
    std::vector<int> waterX;
    std::vector<int> waterY;

    int lavaCount;
    std::vector<int> lavaX;
    std::vector<int> lavaY;
    */

    // Floors
    TileVariant baseFloorVariant;
    int floorCount;
    std::vector<Pos2D> floorGridPos;
    std::vector<TileVariant> floorVariant;

    // Objects
    int sObjectCount;
    std::vector< int > sObjectID;
    std::vector<Pos2D> sObjectPos;
    std::vector<std::string> sObjectData;

    int dObjectCount;
    std::vector< int > dObjectID;
    std::vector<Pos2D> dObjectPos;
    std::vector<std::string> dObjectData;

    int wireCount;
    std::vector<Pos2D> wirePos;
    std::vector<int> wireType;
    std::vector<bool> wirePowered;
    int gateCount;
    std::vector<Pos2D> gatePos;
    std::vector<int> gateType;
    std::vector<Direction> gateFacing;
    std::vector<bool> gatePoweredL;
    std::vector<bool> gatePoweredR;
    std::vector<bool> gatePoweredB;
    
    /*
    int crateCount;
    std::vector<int> crateX;
    std::vector<int> crateY;

    int pressurePlateCount;
    std::vector<int> plateX;
    std::vector<int> plateY;
    std::vector<std::string> plateType;

    int lampCount;
    std::vector<int> lampX;
    std::vector<int> lampY;
    std::vector<std::string> lampType;
    std::vector<bool> lampToggleable;

    int doorCount;
    std::vector<int> doorX;
    std::vector<int> doorY;
    std::vector<std::string> doorFacing;

    int signCount;
    std::vector<int> signX;
    std::vector<int> signY;
    std::vector<std::string> signText;

    int shooterCount;
    std::vector<std::string> shooterType;
    std::vector<int> shooterX;
    std::vector<int> shooterY;
    std::vector<int> shooterOffset;
    std::vector<std::string> shooterFacing;
    std::vector<bool> shooterActive;

    int barbarianCount;
    std::vector<int> barbarianX;
    std::vector<int> barbarianY;
    std::vector<std::string> barbarianType;

    int wizardCount;
    std::vector<int> wizardX;
    std::vector<int> wizardY;
    std::vector<std::string> wizardType;

    int paladinCount;
    std::vector<int> paladinX;
    std::vector<int> paladinY;
    std::vector<std::string> paladinType;

    int zombieCount;
    std::vector<int> zombieX;
    std::vector<int> zombieY;
    std::vector<std::string> zombieType;

    int lichCount;
    std::vector<int> lichX;
    std::vector<int> lichY;
    std::vector<std::string> lichType;

    int wraithCount;
    std::vector<int> wraithX;
    std::vector<int> wraithY;
    std::vector<std::string> wraithType;
    */
};
