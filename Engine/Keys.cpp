#include "Keys.h"

namespace kb
{
    // Helper functions for converting between indices, codes, names and characters
    int index_to_code(int index)
    {
        return keyCodes[index];
    }

    char index_to_char(int index)
    {
        return keyCharacters[index];
    }

    std::string index_to_name(int index)
    {
        return keyNames[index];
    }

    int code_to_index(int code)
    {
        for (int i = 0; i < KEY_COUNT; i++) {
            if (keyCodes[i] == code) {
                return i;
            }
        }
        return -1;
    }

    char code_to_char(int code)
    {
        for (int i = 0; i < KEY_COUNT; i++) {
            if (keyCodes[i] == code) {
                return keyCharacters[i];
            }
        }
        return 0;
    }

    std::string code_to_name(int code)
    {
        for (int i = 0; i < KEY_COUNT; i++) {
            if (keyCodes[i] == code) {
                return keyNames[i];
            }
        }
        return "unknown";
    }

    int name_to_code(std::string name)
    {
        for (int i = 0; i < KEY_COUNT; i++) {
            if (keyNames[i] == name) {
                return keyCodes[i];
            }
        }
        return -1;
    }


}
