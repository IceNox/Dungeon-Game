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
