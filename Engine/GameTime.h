#pragma once

#include <chrono>

using namespace std::chrono;

class TimePoint;
class Duration;

namespace maintime
{
    extern long long int currentGameTime;
    extern long long int updateCount;

    extern time_point<high_resolution_clock> lastTimeSync;
    extern time_point<high_resolution_clock> currentTime;
    extern float clockSpeed;
    extern bool  paused;

    void initialize();
    void update_time();

    bool set_clock_speed(float s);
    bool increase_clock_speed(float s);
    bool decrease_clock_speed(float s);

    void pause();
    void unpause();

    TimePoint now();
}

enum TimeUnits
{
    MICROSECONDS = 1,
    MILLISECONDS = 1000,
    SECONDS         = 1000000,
    MINUTES         = 60000000,
    HOURS         = 3600000000
};

class Duration
{
    long long int d;
public:
    Duration() : d(0) {}
    Duration(long long int td, TimeUnits u = MICROSECONDS) : d(td * u) {}
    Duration(const Duration& td)
    {
        d = td.get_duration();
    }
    void operator =(const Duration& td)
    {
        d = td.get_duration();
    }

    long long int get_duration(TimeUnits u = MICROSECONDS) const { return d / u; }
};

class TimePoint
{
    long long int t;
public:
    TimePoint() : t(maintime::currentGameTime) {}
    TimePoint(long long int tp) : t(tp) {}
    TimePoint(const TimePoint& tp)
    {
        t = tp.get_time();
    }
    void operator =(const TimePoint& tp)
    {
        t = tp.get_time();
    }
    void operator +=(const Duration& d)
    {
        t += d.get_duration();
    }

    long long int get_time(TimeUnits u = MICROSECONDS) const { return t / u; }
};

inline bool operator< (const Duration& d1, const Duration& d2)
{
    return d1.get_duration() < d2.get_duration();
}

inline bool operator> (const Duration& d1, const Duration& d2)
{
    return d1.get_duration() > d2.get_duration();
}

inline bool operator<= (const Duration& d1, const Duration& d2)
{
    return d1.get_duration() <= d2.get_duration();
}

inline bool operator>= (const Duration& d1, const Duration& d2)
{
    return d1.get_duration() >= d2.get_duration();
}

inline bool operator== (const Duration& d1, const Duration& d2)
{
    return d1.get_duration() == d2.get_duration();
}

inline Duration operator- (const TimePoint& t1, const TimePoint& t2)
{
    return t1.get_time() - t2.get_time();
}