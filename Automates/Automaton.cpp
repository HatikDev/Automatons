#include "Automaton.h"
#include "Transition.h"

void Automaton::printAutomaton(const std::string& filename)
{
    std::ofstream file(filename);
    for (auto* state : states)
        state->printState(file);

    for (auto* transition : transitions)
        transition->printTransition(file);

    file.close();
}
