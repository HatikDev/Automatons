#include "Constants.h"
#include "Utils.h"

#include <algorithm>

bool isSpecialSymbol(char symbol)
{
    return std::find(SPECIALSYMBOLS.begin(), SPECIALSYMBOLS.end(), std::string(1, symbol)) != SPECIALSYMBOLS.end();
}

bool isSpecialSymbol(std::string symbol)
{
    return std::find(SPECIALSYMBOLS.begin(), SPECIALSYMBOLS.end(), symbol) != SPECIALSYMBOLS.end();
}