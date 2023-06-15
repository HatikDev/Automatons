#include "Ast.h"
#include "Automaton.h"
#include "Node.h"
#include "State.h"
#include "Transition.h"

#include <cassert>

void makeElementalAutomaton(Node* node)
{
    State* startState = new State(true, false);
    Automaton* automaton = new Automaton();
    automaton->addState(startState);
    node->setAutomaton(automaton);

    std::string value = node->getValue();
    State* prevState = startState;
    State* currentState = nullptr;
    assert(value.length());
    for (auto symbol : value)
    {
        currentState = new State(false, false);
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
    }
    else
    {
        auto value = node->getValue();
        if (value == "|_")
        {
            Automaton* automaton1 = node->getChildLeft()->getAutomaton();
            Automaton* automaton2 = node->getChildRight()->getAutomaton();

            State* startState1 = automaton1->getStartState();
            startState1->removeFlag(STARTSTATE);
            State* startState2 = automaton2->getStartState();
            startState2->removeFlag(STARTSTATE);

            State* startState = new State(true, false);

            Automaton* automaton = new Automaton();
            Transition* transition1 = new Transition(startState, startState1, '?');
            Transition* transition2 = new Transition(startState, startState2, '?');
            automaton->addState(startState);
            automaton->addTransition(transition1);
            automaton->addTransition(transition2);
            automaton->addAutomaton(automaton1);
            automaton->addAutomaton(automaton2);
        }
        else if (value == "*_")
        {
            
        }
        else if (value == "()")
        {

        }
    }

    return;
}

void removeTree(Node* root)
{
    if (root->getChildLeft())
        removeTree(root->getChildLeft());
    if (root->getChildRight())
        removeTree(root->getChildRight());
    delete root;
}

int main(int argc, char* argv[])
{
    auto ast = makeAST("123(abc|456|789)*");
    printAST(ast[0]);

    assert(ast.size() == 1);

    //printASTInFile("ast.txt", ast[0]);
    makeNFA(ast[0]);

    removeTree(ast[0]);
    return 0;
}
