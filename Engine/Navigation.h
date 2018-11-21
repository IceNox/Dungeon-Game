#pragma once

#ifndef POSITION_H
#define POSITION_H

#include <cmath>
#include <unordered_map>

enum Direction
{
    NO_DIRECTION = -1,
    UP,
    RIGHT,
    DOWN,
    LEFT
};

class Pos2D
{
public:
    int x;
    int y;

    Pos2D() {}
    Pos2D(int x, int y) : x(x), y(y) {}
    Pos2D(int s) : x(s), y(s) {}
    Pos2D(const Pos2D &c)
    {
        this->x = c.x;
        this->y = c.y;
    }

    void move(Direction dir, int amount)
    {
        switch (dir)
        {
        case UP:
            y -= amount;
            break;
        case RIGHT:
            x += amount;
            break;
        case DOWN:
            y += amount;
            break;
        case LEFT:
            x -= amount;
            break;
        }
    }

    int index(int width)
    {
        return y * width + x;
    }

    int index(int width, Direction dir, int dist)
    {
        switch (dir)
        {
            case UP:
                return (y - dist) * width + x;
            case RIGHT:
                return y * width + x + dist;
            case DOWN:
                return (y + dist) * width + x;
            case LEFT:
                return y * width + x - dist;
        }
    }

    Direction vdirection()
    {
        if (x == 0 && y == 0)
            return NO_DIRECTION;

        if (abs(x) >= abs(y))
            if (x > 0)
                return RIGHT;
            else
                return LEFT;
        else
            if (y > 0)
                return UP;
            else
                return DOWN;
    }

    Pos2D operator= (const Pos2D &p)
    {
        this->x = p.x;
        this->y = p.y;
        return *this;
    }

    Pos2D operator+ (const Pos2D &p)
    {
        return Pos2D
        (
            this->x + p.x,
            this->y + p.y
        );

        this->x += p.x;
        this->y += p.y;
        return *this;
    }

    Pos2D operator- (const Pos2D &p)
    {
        return Pos2D
        (
            this->x - p.x,
            this->y - p.y
        );

        this->x -= p.x;
        this->y -= p.y;
        return *this;
    }

    Pos2D operator* (int t)
    {
        return Pos2D
        (
            this->x * t,
            this->y * t
        );

        this->x *= t;
        this->y *= t;
        return *this;
    }

    Pos2D operator* (float t)
    {
        return Pos2D
        (
            this->x * t,
            this->y * t
        );

        this->x *= t;
        this->y *= t;
        return *this;
    }

    Pos2D operator/ (int t)
    {
        return Pos2D
        (
            this->x / t,
            this->y / t
        );

        this->x /= t;
        this->y /= t;
        return *this;
    }

    Pos2D operator/ (float t)
    {
        return Pos2D
        (
            this->x / t,
            this->y / t
        );

        this->x /= t;
        this->y /= t;
        return *this;
    }
};

inline bool operator ==(const Pos2D &p1, const Pos2D &p2)
{
    return (p1.x == p2.x && p1.y == p2.y);
}

inline bool operator< (const Pos2D &p1, const Pos2D &p2)
{
    return (p1.x < p2.x || p1.y < p2.y);
}

inline bool operator> (const Pos2D &p1, const Pos2D &p2)
{
    return (p1.x > p2.x || p1.y > p2.y);
}

void rotate_direction(Direction &dir, int t);

extern std::unordered_map<std::string, Direction> directionMap;
extern std::unordered_map<Direction, Pos2D> directionVectorMap;

#endif // POSITION_H
