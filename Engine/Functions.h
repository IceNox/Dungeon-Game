#pragma once

#include "Position.h"

#include <Windows.h>

#include <string>

// Number manipulation
int absv(int v);

// Geometry
int distance(Pos2D pos1, Pos2D pos2);
float distance_real(Pos2D pos1, Pos2D pos2);
int distance_manh(Pos2D pos1, Pos2D pos2);

// Strings
std::string wstring_to_string(std::wstring ws);
void		split_str(std::string str, std::vector<std::string> &v);
int			str_to_int(std::string str, int base = 10);
std::string int_to_str(int num);
int			find_text_in_vec(std::string str, std::vector<std::string> vec);
int			find_text_in_arr(std::string str, std::string arr[], int size);

// Bounds
bool add_bounds(Pos2D &pos, const RECT &bounds);
bool add_bounds(int &value, const int &min_v, const int &max_v);
bool add_bounds(float &value, const float &min_v, const float &max_v);
bool check_bounds(const Pos2D &pos, const RECT &bounds);
bool check_bounds(const int &value, const int &min_v, const int &max_v);
bool check_bounds(const float &value, const float &min_v, const float &max_v);