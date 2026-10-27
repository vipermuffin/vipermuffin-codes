#pragma once
// Everybody Codes 2025 - Quest 4
// Declare the pieces you want to unit test (input type, parse(), one step, ...)
// and define them in q2025_04.cpp inside the same namespace.

#include "vmc/vmc.h"
#include <vector>
#include <numeric>
#include <functional>

namespace q2025_04 {
template <typename T>
T calculateGearRatio(const std::vector<T>& gears) {
    std::vector<T> ratios{1};
    auto itr = std::next(gears.begin());
    while(itr != gears.end()) {
        ratios.push_back(*std::prev(itr) / *itr);
        itr = std::next(itr);
    }
    return std::accumulate(ratios.begin(), ratios.end(), static_cast<T>(1), std::multiplies<>());
}

template <typename T>
T calculatePairedGearRatio(const std::vector<std::pair<T,T>>& gears) {
    std::vector<T> ratios{1};
    auto itr = std::next(gears.begin());
    while(itr != gears.end()) {
        ratios.push_back(std::prev(itr)->second / itr->first);
        itr = std::next(itr);
    }
    return std::accumulate(ratios.begin(), ratios.end(), static_cast<T>(1), std::multiplies<>());
}
}  // namespace q2025_04
