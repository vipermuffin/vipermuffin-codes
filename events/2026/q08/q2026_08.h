#pragma once
// Everybody Codes 2026 - Quest 8
// Declare the pieces you want to unit test (input type, parse(), one step, ...)
// and define them in q2026_08.cpp inside the same namespace.

#include "vmc/vmc.h"
#include <unordered_map>

namespace q2026_08 {
    int calcLineEnergy(const std::vector<int>&, const std::unordered_map<vmc::Point, int>&);

}  // namespace q2026_08
