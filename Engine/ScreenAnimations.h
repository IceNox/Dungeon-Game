#pragma once

#include "Colors.h"

#include <chrono>
#include <cstdlib>

using namespace std::chrono;

class ScreenAnimations
{
public:
    time_point<system_clock> classStart;

    int windowWidth;
    int windowHeight;

    // Screen shake animation variables
    time_point<system_clock> SAstart;

    int x1, y1;
    int x2, y2;
    int x3, y3;
    int x4, y4;

    // Slow effect animation variables
    time_point<system_clock> SEAstart;

    // Death effect variables
    time_point<system_clock> DEstart;

    // Level end effect variables
    time_point<system_clock> EEstart;

public:
    ScreenAnimations();

    void set_window_parameters(int w, int h);

    void start_shake(int strength);
    void apply_shake(int &startX, int &startY);

    void start_slow();
    void apply_slow(Color pSysBuffer[]);

    void start_death_effect();
    void apply_death_effect(Color pSysBuffer[]);

    void start_end_effect();
    void apply_end_effect(Color pSysBuffer[]);
};