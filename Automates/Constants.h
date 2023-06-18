#pragma once

#include <stdint.h>
#include <string>
#include <vector>

constexpr uint8_t NOFLAGS = 0;
constexpr uint8_t STARTSTATE = 1;
constexpr uint8_t ENDSTATE = 2;

const std::string OPENBRACKET = "(";
const std::string CLOSESTBRACKET = ")";
const std::string STAR = "*";
const std::string OR = "|";
const std::vector<std::string> SPECIALSYMBOLS = { OPENBRACKET, CLOSESTBRACKET, STAR, OR };
