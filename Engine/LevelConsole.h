#pragma once

#include "Keys.h"

#include "LevelMessage.h"

#include <vector>
#include <string>

// Constants
const std::string PREFIX = "> ";

class LevelConsole
{
private:
    kb::SmartText text;
    bool opened;

public:
    LevelConsole()
        :
        text(PREFIX),
        opened(false)
    {
        text.setWhitelist();
    }

    void update_console(kb::Keys &k, std::vector<LevelMessage> &messages)
    {
        if (!opened) return;

        text.update_text(k);

        if (k.key_clicked(kb::KC_ENTER))
        {
            messages.push_back(LevelMessage("console " + text.get_content()));
            text.clear_text();
        }

        if (k.key_state(kb::KC_NUM_0)) {
            messages.push_back(LevelMessage("console animation 144 144 60 [ground=false] [fholdtime=1000]"));
            //messages.push_back(LevelMessage("console particle 144 144 [velocity=100;0] [acceleration=-50;0] [elength=2000] [flength=1000] [color=255;0;0] [rect=20;10]"));
            text.clear_text();
        }
    }

    void open()
    {
        opened = true;
        text.clear_text();
    }

    void close()
    {
        opened = false;
    }

    bool is_opened()
    {
        return opened;
    }

    std::string get_text()
    {
        return text.get_content();
    }
};
