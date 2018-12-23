#pragma once

#include "Keys.h"

#include "LevelMessage.h"

#include <vector>
#include <string>

class LevelConsole
{
    kb::SmartText text;
    bool opened;
public:

    LevelConsole()
    {
        close();
    }

    void update_console(kb::Keys &k, std::vector<LevelMessage> &messages)
    {
        if (!opened) return;

        text.update_text(k);

        if (k.key_state(kb::KC_ENTER))
        {
            messages.push_back(LevelMessage("console " + text.get_text()));
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
        return text.get_text();
    }
};
