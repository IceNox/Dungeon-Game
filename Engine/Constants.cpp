#include "Constants.h"

void rotate_direction(Direction &dir, int t)
{
    dir = static_cast<Direction>((dir + t) % 4);
}

std::string action_string(int ma)
{
    return action_strings[ma];
}
