#pragma once

#include "Constants.h"

#include <stdint.h>

class State
{
public:
    State(bool isStartState, bool isEndState)
    {
        id = nextId++;
        flags = 0;

        if (isStartState)
            flags |= STARTSTATE;
        if (isEndState)
            flags |= ENDSTATE;
    }
    ~State() {}

    uint64_t getId()
    {
        return id;
    }

    uint8_t getFlags()
    {
        return flags;
    }

    void addFlag(uint8_t flag)
    {
        flags |= flag;
    }

    void removeFlag(uint8_t flag)
    {
        if (flags & flag)
            flags ^= flag;
    }

private:
    uint64_t id;
    uint8_t flags;
    static uint64_t nextId;
};