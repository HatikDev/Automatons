#pragma once

#include "State.h"

#include <vector>

class State;
class Transition;

class Automaton
{
public:
    Automaton()
    {}
    Automaton(const Automaton&) = delete;
    ~Automaton() {}

    void addState(State* state)
    {
        states.push_back(state);

        if (state->getFlags() & STARTSTATE)
            startState = state;
        if (state->getFlags() & ENDSTATE)
            endState = state;
    }

    void addTransition(Transition* transition)
    {
        transitions.push_back(transition);
    }

    std::vector<State*>& getStates()
    {
        return states;
    }

    std::vector<Transition*>& getTransitions()
    {
        return transitions;
    }

    State* getStartState()
    {
        return startState;
    }

    State* getEndState()
    {
        return endState;
    }

    void addAutomaton(Automaton* automaton)
    {
        auto& states = automaton->getStates();
        auto& transitions = automaton->getTransitions();
    }

private:
    std::vector<State*> states;
    State* startState;
    State* endState;
    std::vector<Transition*> transitions;
};