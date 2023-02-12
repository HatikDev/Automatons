#pragma once

#include <string>

class Automaton;

class Node
{
public:
    Node(std::string value, Node* childLeft = NULL, Node* childRight = NULL)
        : value{value}, childLeft{childLeft}, childRight{childRight}, automaton{NULL}
    {}
    Node(const Node&) = delete;
    ~Node() {}

    std::string getValue()
    {
        return value;
    }

    void setValue(const std::string& value)
    {
        this->value = value;
    }

    Node* getChildLeft()
    {
        return childLeft;
    }

    void setChildLeft(Node* node)
    {
        childLeft = node;
    }

    Node* getChildRight()
    {
        return childRight;
    }

    void setChildRight(Node* node)
    {
        childRight = node;
    }

    void setAutomaton(Automaton* automaton)
    {
        this->automaton = automaton;
    }

    Automaton* getAutomaton()
    {
        return automaton;
    }

private:
    std::string value;
    Node* childLeft;
    Node* childRight;
    Automaton* automaton;
};