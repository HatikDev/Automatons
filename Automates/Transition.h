#pragma once

class Transition
{
public:
    Transition(State* state1, State* state2, char character)
    {
        m_stateId1 = state1->getId();
        m_stateId2 = state2->getId();
        m_character = character;
    }
    //Transition(const Transition&) = delete;
    ~Transition() {}

private:
    uint64_t m_stateId1;
    uint64_t m_stateId2;
    char m_character;
};
