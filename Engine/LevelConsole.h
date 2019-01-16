#pragma once

#include "LevelMessage.h"
#include "SmartText.h"

#include <vector>
#include <string>

// Constants
const std::string PREFIX = "> ";

// Macros
const Key SUBMIT = Key(VK_RETURN, HELD);

class LevelConsole
{
private:
    SmartText text;
    bool opened;

public:
    LevelConsole()
        :
        text(PREFIX),
        opened(false)
    {
        text.setWhitelist();
    }

    void update_console(Key &k, std::vector<LevelMessage> &messages)
    {
        if (!opened) return;

        text.update_text(k);

        if (k == SUBMIT)
        {
            messages.push_back(LevelMessage("console " + text.get_content()));
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
