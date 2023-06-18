#pragma once

#include "Constants.h"

#include <stdint.h>

class State
{
public:
    State(uint8_t flags) : m_flags{ flags }, m_id{ nextId++ }
    {}
    ~State() {}

    uint64_t getId()
    {
        return m_id;
    }

    uint8_t getFlags()
    {
        return m_flags;
    }

    void addFlag(uint8_t flag)
    {
        m_flags |= flag;
    }

    void removeFlag(uint8_t flag)
    {
        if (m_flags & flag)
            m_flags ^= flag;
    }

private:
    uint64_t m_id;
    uint8_t m_flags;
    static uint64_t nextId;
};