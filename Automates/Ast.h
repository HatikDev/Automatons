#pragma once

#include "Node.h"

#include <string>
#include <vector>

void printAST(Node* tree, size_t counter = 0);

void printASTInFile(std::string filename, Node* root);

std::vector<Node*> makeAST(std::string regex);
