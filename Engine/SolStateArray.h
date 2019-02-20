#pragma once

#include "Sol.hpp"

class SolStateArray
{
    size_t objcount;
    size_t capacity;
    sol::state** states;
    bool* occupied;

    void expand()
    {
        const size_t EXPAND_BY = 50;

        sol::state** newstates = new sol::state*[capacity + EXPAND_BY];
        bool* newoccupied = new bool[capacity + EXPAND_BY];

        for (unsigned i = 0; i < capacity; i++) {
            newstates[i] = states[i];
            newoccupied[i] = occupied[i];
        }

        capacity += EXPAND_BY;

        delete[] states;
        delete[] occupied;

        states = newstates;
        occupied = newoccupied;
    }

public:
    SolStateArray()
    {
        // Reserve 100 places and expand later if necessary 
        states = new sol::state*[100];
        occupied = new bool[100];

        for (int i = 0; i < 100; i++) {
            occupied[i] = false;
        }

        objcount = 0;
        capacity = 100;
    }

    int AddState()
    {
        // Resize if not enough space
        if (objcount == capacity) {
            expand();
        }

        for (unsigned i = 0; i < capacity; i++) {
            if (!occupied[i]) {
                states[i] = new sol::state;
                occupied[i] = true;

                objcount++;
                return i;
            }
        }
    }

    void ReleaseObj(int index)
    {
        if (occupied[index]) {
            states[index] = nullptr;
            occupied[index] = false;
            objcount--;
        }
    }

    ~SolStateArray()
    {
        delete[] states;
        delete[] occupied;
    }

    sol::state* operator[](int index)
    {
        if (index < 0 && index >= capacity) return nullptr;
        if (!occupied[index]) return nullptr;

        return states[index];
    }
};
