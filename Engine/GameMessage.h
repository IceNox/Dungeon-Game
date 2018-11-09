#pragma once

#include "Constants.h"
#include "HelperStructs.h"

#include <string>
#include <vector>

class GameMessage
{
public:
    virtual ~GameMessage() = default;

    virtual void handle_msg
    (
        GameStateData &gsd
    )
    {
        
    }
};

class MsgEnterLevel : public GameMessage
{
    LevelType    lType;
    std::string lName;
public:
    MsgEnterLevel(LevelType lt, std::string ln) : lType(lt), lName(ln) {}

    virtual void handle_msg
    (
        GameStateData &gsd
    )
    {
        gsd.inLevel       = true;
        gsd.enterLevel = true;
        gsd.lType = lType;
        gsd.lName = lName;
    }
};

class MsgEnterMenu : public GameMessage
{
public:
    MsgEnterMenu() {}

    virtual void handle_msg
    (
        GameStateData &gsd
    )
    {
        gsd.enterMenu = true;
    }
};