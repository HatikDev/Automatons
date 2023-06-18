#include "Ast.h"
#include "Automaton.h"
#include "Node.h"
#include "State.h"
#include "Transition.h"

#include <cassert>

void makeElementalAutomaton(Node* node)
{
    State* startState = new State(STARTSTATE);
    Automaton* automaton = new Automaton();
    automaton->addState(startState);
    node->setAutomaton(automaton);

    std::string value = node->getValue();
    State* prevState = startState;
    State* currentState = nullptr;
    assert(value.length());
    for (auto symbol : value)
    {
        currentState = new State(NOFLAGS);
        Transition* transition = new Transition(prevState, currentState, symbol);
        automaton->addState(currentState);
        automaton->addTransition(transition);

        prevState = currentState;
    }
    currentState->addFlag(ENDSTATE);
}

void makeNFA(Node* node)
{
    std::vector<State> states;
    std::vector<Transition> transitions;

    if (node->getChildLeft())
        makeNFA(node->getChildLeft());
    if (node->getChildRight())
        makeNFA(node->getChildRight());

    if (!node->getChildLeft() && !node->getChildRight())
    {
        makeElementalAutomaton(node);
        return;
    }

    auto value = node->getValue();
    if (value == "|_")
    {
        Automaton* automaton1 = node->getChildLeft()->getAutomaton();
        Automaton* automaton2 = node->getChildRight()->getAutomaton();

        State* startState1 = automaton1->getStartState();
        startState1->removeFlag(STARTSTATE);
        State* startState2 = automaton2->getStartState();
        startState2->removeFlag(STARTSTATE);

        State* startState = new State(STARTSTATE);

        Automaton* automaton = new Automaton();
        Transition* transition1 = new Transition(startState, startState1, '?');
        Transition* transition2 = new Transition(startState, startState2, '?');
        automaton->addState(startState);
        automaton->addTransition(transition1);
        automaton->addTransition(transition2);
        automaton->addAutomaton(automaton1);
        automaton->addAutomaton(automaton2);

        node->setAutomaton(automaton);
        return;
    }
    else if (value == "*_")
    {
        State* startState = new State(STARTSTATE);
        State* endState = new State(ENDSTATE);

        Automaton* automaton = node->getChildLeft()->getAutomaton();
        State* automatonStartState = automaton->getStartState();
        State* automatonEndState = automaton->getEndState();
        automatonStartState->removeFlag(STARTSTATE);
        automatonEndState->removeFlag(ENDSTATE);

        Transition* emptyTransition = new Transition(startState, endState, '?');
        Transition* firstAutomatonTransition = new Transition(startState, automatonStartState, '?');
        Transition* secondAutomatonTransition = new Transition(automatonEndState, endState, '?');
        Transition* backEmptyTransition = new Transition(endState, startState, '?');

        Automaton* nodeAutomaton = new Automaton();
        nodeAutomaton->addState(automatonStartState);
        nodeAutomaton->addState(automatonEndState);

        nodeAutomaton->addTransition(emptyTransition);
        nodeAutomaton->addTransition(firstAutomatonTransition);
        nodeAutomaton->addTransition(secondAutomatonTransition);
        nodeAutomaton->addTransition(backEmptyTransition);

        nodeAutomaton->addAutomaton(automaton);
        node->setAutomaton(automaton);
        return;
    }
    else if (value == "()")
    {

    }

    return;
}

void removeTree(Node* root)
{
    delete root;
}

int main(int argc, char* argv[])
{
    auto ast = makeAST("123(abc|456|789)*");
    printAST(ast[0]);

    assert(ast.size() == 1);

    printASTInFile("ast.txt", ast[0]);
    //makeNFA(ast[0]);

    removeTree(ast[0]);
    return 0;
}
