#pragma once
// Everybody Codes 2026 - Quest 3
// Declare the pieces you want to unit test (input type, parse(), one step, ...)
// and define them in q2026_03.cpp inside the same namespace.

#include "vmc/vmc.h"

namespace q2026_03 {
long convertVoteToDecimalNotation(const std::string_view);
bool isValidIso2(const std::string_view);
}  // namespace q2026_03
