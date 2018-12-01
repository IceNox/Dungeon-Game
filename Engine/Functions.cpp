#include "Functions.h"

#include "Constants.h"
#include "GlobalData.h"

#include <cmath>
#include <locale>
#include <codecvt>
#include <bitset>
#include <sstream>
#include <fstream>

int absv(int v)
{
    int mask = v >> 31;
    return (mask ^ v) - mask;
}

int distance(Pos2D pos1, Pos2D pos2)
{
    return trunc(sqrt((pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y)));
}

float distance_real(Pos2D pos1, Pos2D pos2)
{
    return sqrt(((pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y)));
}

int distance_manh(Pos2D pos1, Pos2D pos2)
{
    return absv(pos1.x - pos2.x) + absv(pos1.y - pos2.y);
}

// Strings
std::string wstring_to_string(std::wstring ws)
{
    //setup converter
    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;

    //use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
    return converter.to_bytes(ws);
}

void split_str(std::string str, std::vector<std::string> &v, char split)
{
    std::string t; t.reserve(10);
    for (int i = 0; i < str.length(); i++) {
        if (str[i] != split) {
            t.push_back(str[i]);
        }
        else if (str[i] == split && t.length() > 0) {
            v.push_back(t);
            t.clear();
        }
    }

    if (t.length() > 0) {
        v.push_back(t);
        t.clear();
    }
}

int str_to_int(std::string str, int base)
{
    bool negative = false;
    
    std::vector<int> nums;
    nums.reserve(9);

    // Get numbers from string
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == '-') {
            negative = true;
        }
        else {
            int val = str[i] - 48;

            if (val >= 0 && val <= 9) {
                nums.push_back(val);
            }
        }

        if (nums.size() > 8) continue;
    }
    if (nums.size() == 0) return 0;

    // Combine digits into a number
    int size = nums.size();
    int mult = 1;
    int fnum = 0;

    for (int i = size - 1; i >= 0; i--) {
        fnum += nums[i] * mult;
        mult *= base;
    }

    if (negative) fnum = -fnum;

    return fnum;
}

std::string int_to_str(int num)
{
    std::ostringstream ss;
    ss << num;
    return ss.str();
}

int find_text_in_vec(std::string str, std::vector<std::string> vec)
{
    for (int i = 0; i < vec.size(); i++)
        if (vec[i] == str)
            return i;

    return -1;
}

int find_text_in_arr(std::string str, std::string arr[], int size)
{
    for (int i = 0; i < size; i++)
        if (arr[i] == str)
            return i;

    return -1;
}

std::string extract_str_until(std::string str, char tc)
{
    std::string s;
    s.reserve(10);

    for (unsigned i = 0; i < str.length(); i++) {
        if (str[i] != tc)
            s.push_back(str[i]);
        else
            break;
    }

    return s;
}

void erase_str_until(std::string &str, char tc)
{
    int l = extract_str_until(str, tc).length();

    str.erase(str.begin(), str.begin() + l);
    while (str[0] == tc) {
        str.erase(str[0]);
    }
}

std::string exer_str_until(std::string &str, char tc)
{
    std::string s = extract_str_until(str, tc);

    str.erase(str.begin(), str.begin() + s.length());
    while (str[0] == tc) {
        str.erase(str.begin(), str.begin() + 1);
    }

    return s;
}

// Bounds
bool add_bounds(Pos2D &pos, const RECT &bounds)
{
    bool inBounds = true;
    // x
    if        (pos.x < bounds.left) {
        pos.x = bounds.left;
        inBounds = false;
    }
    else if (pos.x > bounds.right) {
        pos.x = bounds.right;
        inBounds = false;
    }
    // y
    if        (pos.y < bounds.top) {
        pos.y = bounds.top;
        inBounds = false;
    }
    else if (pos.y > bounds.bottom) {
        pos.y = bounds.bottom;
        inBounds = false;
    }

    return inBounds;
}

bool add_bounds(int &value, const int &min_v, const int &max_v)
{
    if        (value < min_v) {
        value = min_v;
        return false;
    }
    else if (value > max_v) {
        value = max_v;
        return false;
    }

    return true;
}

bool add_bounds(float &value, const float &min_v, const float &max_v)
{
    if (value < min_v) {
        value = min_v;
        return false;
    }
    else if (value > max_v) {
        value = max_v;
        return false;
    }

    return true;
}

bool check_bounds(const Pos2D &pos, const RECT &bounds)
{
    // x
    if (pos.x < bounds.left)
        return false;
    if (pos.x > bounds.right)
        return false;
    // y
    if (pos.y < bounds.top)
        return false;
    if (pos.y > bounds.bottom)
        return false;

    return true;
}

bool check_bounds(const int &value, const int &min_v, const int &max_v)
{
    if (value < min_v)
        return false;
    if (value > max_v)
        return false;

    return true;
}

bool check_bounds(const float &value, const float &min_v, const float &max_v)
{
    if (value < min_v)
        return false;
    if (value > max_v)
        return false;

    return true;
}

int load_sprite(std::string path, std::string name)
{
    sprites.push_back(Sprite(path, name));
    return sprites.size() - 1; // Return array index of loaded sprite
}
