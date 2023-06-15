#include "Ast.h"
#include "Constants.h"
#include "Utils.h"

#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>
#include <queue>

namespace
{
std::vector<Node*> divideInLexems(std::string regex)
{
    std::string current = "";
    std::vector<Node*> lexems;
    for (auto symbol : regex)
    {
        if (!isSpecialSymbol(symbol))
        {
            current += symbol;
        }
        else
        {
            if (!current.empty())
            {
                Node* node = new Node(current);//Node{ current, nullptr, nullptr, nullptr, nullptr };
                lexems.push_back(node);
                current = "";
            }
            Node* node = new Node(std::string(1, symbol));//Node{ std::string(1, symbol), nullptr, nullptr, nullptr, nullptr };
            lexems.push_back(node);
        }
    }
    if (!current.empty())
    {
        Node* node = new Node(current);//{ current, nullptr, nullptr, nullptr, nullptr };
        lexems.push_back(node);
    }
    return lexems;
}

std::pair<size_t, size_t> findInternalBrackets(std::vector<Node*>& lexems)
{
    size_t priority = 0;
    size_t maxPriority = 0;
    size_t openIndexMaxPriority = 0;
    size_t closeIndexMaxPriority = 0;

    // try to find open bracket

    for (size_t i = 0; i < lexems.size(); ++i)
    {
        Node& currentNode = *lexems[i];
        if (currentNode.getValue() == OPENBRACKET)
        {
            ++priority;
            if (priority > maxPriority)
            {
                maxPriority = priority;
                openIndexMaxPriority = i;
            }
        }
        else if (currentNode.getValue() == CLOSESTBRACKET)
        {
            --priority;
        }
    }

    // let's find closest bracket

    maxPriority = 0;
    for (int i = lexems.size() - 1; i >= 0; --i)
    {
        Node& currentNode = *lexems[i];
        if (currentNode.getValue() == CLOSESTBRACKET)
        {
            ++priority;
            if (priority > maxPriority)
            {
                maxPriority = priority;
                closeIndexMaxPriority = i;
            }
        }
        else if (currentNode.getValue() == OPENBRACKET)
        {
            --priority;
        }
    }

    return { openIndexMaxPriority, closeIndexMaxPriority };
}

void makeTreeWithBrackets(std::vector<Node*>& lexems, size_t openIndex, size_t closeIndex) // rewrite this function
{
    Node& openNode = *lexems[openIndex];

    openNode.setValue("()");
    openNode.setChildLeft(lexems[openIndex + 1]);
    openNode.setChildRight(nullptr);

    lexems[openIndex + 1]->setParent(&openNode);

    lexems[openIndex + 1] = nullptr;
    lexems[closeIndex] = nullptr;

    std::erase_if(lexems, [](Node* node){ return !node; });
}

void makeTreeWithStar(std::vector<Node*>& lexems, size_t openIndex, size_t closeIndex)
{
    for (auto it = lexems.begin() + openIndex + 1; it != lexems.begin() + closeIndex; ++it)
    {
        if (!*it)
            continue;

        Node& currentNode = **it;
        if (currentNode.getValue() == STAR)
        {
            currentNode.setValue("*_");
            currentNode.setChildLeft(*(it - 1));
            (*(it - 1))->setParent(&currentNode);
            *(it - 1) = nullptr;
            currentNode.setChildRight(nullptr);
        }
    }

    std::erase_if(lexems, [](Node* node){ return !node; });
}

void makeTreeWithConcat(std::vector<Node*>& lexems, size_t openIndex, size_t closeIndex)
{
    for (auto it = lexems.begin() + openIndex + 1; it != lexems.begin() + closeIndex - 1; ++it)
    {
        if (!*it)
            continue;

        Node& currentNode = **it;
        Node& nextNode = **(it + 1);
        if (!isSpecialSymbol(currentNode.getValue()) && !isSpecialSymbol(nextNode.getValue()))
        {
            Node* node = new Node("concat", *it, *(it + 1));//{"concat", *it, *(it + 1)};

            (*it)->setParent(node);
            (*it + 1)->setParent(node);

            *it = node;
            *(it + 1) = nullptr;
        }
    }

    std::erase_if(lexems, [](Node* node){ return !node; });
}

void makeTreeWithOr(std::vector<Node*>& lexems, size_t openIndex, size_t closeIndex)
{
    bool isChanged = true;
    size_t removedItems = 0;
    while (isChanged)
    {
        isChanged = false;
        auto firstIt = lexems.begin() + (openIndex + 1);
        auto lastIt = lexems.begin() + (closeIndex - removedItems);
        for (auto it = firstIt; it != lastIt; ++it)
        {
            if (!*it)
                continue;

            Node& currentNode = **it;
            if (currentNode.getValue() == OR)
            {
                currentNode.setValue("|_");

                currentNode.setChildLeft(*(it - 1));
                (*(it - 1))->setParent(&currentNode);
                *(it - 1) = nullptr;

                currentNode.setChildRight(*(it + 1));
                (*(it + 1))->setParent(&currentNode);
                *(it + 1) = nullptr;

                isChanged = true;
                break;
            }
        }
        removedItems += std::count(firstIt, lastIt, (Node*)nullptr);
        std::erase_if(lexems, [](Node* node){ return !node; });
    }
}

std::string getSpaces(size_t indent, char delimeter = '|', size_t tabWidth = 4)
{
    std::string result;
    for (size_t i = 0; i < indent; ++i)
    {
        result += std::string(tabWidth, ' ');
        result += '|';
    }
    return result;
}
} // namespace

void printAST(Node* tree, size_t level)
{
    std::cout << getSpaces(level) << tree->getValue() << std::endl;
    if (tree->getChildLeft())
        printAST(tree->getChildLeft(), level + 1);
    if (tree->getChildRight())
        printAST(tree->getChildRight(), level + 1);
}

void printASTInFile(std::string filename, Node* tree)
{
    if (!tree)
        throw std::runtime_error("tree is empty");

    std::ofstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("can't open file");

    // id:%id% left:%left% right:%right% value:%value%

    std::queue<Node*> nodes;
    nodes.push(tree);
    while (!nodes.empty())
    {
        Node* currentNode = nodes.front();
        Node* leftNode = currentNode->getChildLeft();
        Node* rightNode = currentNode->getChildRight();
        const std::string& value = currentNode->getValue();
        
        nodes.pop();

        file << "id:" << currentNode
             << " left:" << leftNode << " right:" << rightNode
             << " value:" << value << std::endl;

        if (leftNode)
            nodes.push(leftNode);
        if (rightNode)
            nodes.push(rightNode);
    }

    file.close();
}

std::vector<Node*> makeAST(std::string regex)
{
    regex = "(" + regex + ")";
    auto lexems = divideInLexems(regex);

    size_t openIndex = 0;
    size_t closeIndex = 1;
    while (openIndex != closeIndex)
    {
        std::tie(openIndex, closeIndex) = findInternalBrackets(lexems);
        if (openIndex == closeIndex)
            break;
        if (closeIndex - openIndex == 2)
        {
            makeTreeWithBrackets(lexems, openIndex, closeIndex);
            continue;
        }
        makeTreeWithStar(lexems, openIndex, closeIndex);

        std::tie(openIndex, closeIndex) = findInternalBrackets(lexems);
        if (openIndex == closeIndex)
            break;
        if (closeIndex - openIndex == 2)
        {
            makeTreeWithBrackets(lexems, openIndex, closeIndex);
            continue;
        }
        makeTreeWithConcat(lexems, openIndex, closeIndex);

        std::tie(openIndex, closeIndex) = findInternalBrackets(lexems);
        if (openIndex == closeIndex)
            break;
        if (closeIndex - openIndex == 2)
        {
            makeTreeWithBrackets(lexems, openIndex, closeIndex);
            continue;
        }
        makeTreeWithOr(lexems, openIndex, closeIndex);
    }

    return lexems;
}
