#pragma once

#include <string>

class Automaton;

class Node
{
public:
    Node(std::string value, Node* childLeft = NULL, Node* childRight = NULL)
        : m_id{globalID++}, m_value{value}, m_childLeft{childLeft}, m_childRight{childRight}, m_automaton{NULL}
    {}
    Node(const Node&) = delete;
    ~Node() {}

    uint64_t getID()
    {
        return m_id;
    }

    std::string getValue()
    {
        return m_value;
    }

    void setValue(const std::string& value)
    {
        this->m_value = value;
    }

    Node* getChildLeft()
    {
        return m_childLeft;
    }

    void setChildLeft(Node* node)
    {
        m_childLeft = node;
    }

    Node* getChildRight()
    {
        return m_childRight;
    }

    void setChildRight(Node* node)
    {
        m_childRight = node;
    }

    Node* getParent()
    {
        return m_parent;
    }

    void setParent(Node* parent)
    {
        m_parent = parent;
    }

    void setAutomaton(Automaton* automaton)
    {
        m_automaton = automaton;
    }

    Automaton* getAutomaton()
    {
        return m_automaton;
    }

private:
    uint64_t m_id;
    std::string m_value;
    Node* m_childLeft;
    Node* m_childRight;
    Node* m_parent;
    Automaton* m_automaton;

    static uint64_t globalID;
};