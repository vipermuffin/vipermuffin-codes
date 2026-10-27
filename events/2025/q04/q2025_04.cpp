// Everybody Codes 2025 - Quest 4
// Approach write-up: NOTES.md in this folder.

#include "q2025_04.h"
#include <iostream>

namespace q2025_04 {

// Shared parsing and core logic. The three parts usually build on one idea
// and scale it up, so keep part1..3 thin and test the pieces from here.

}  // namespace q2025_04

namespace {

using namespace q2025_04;

vmc::Answer part1([[maybe_unused]] std::string_view input) {
    auto gears = vmc::ints<double>(input);
    return {std::to_string(static_cast<long>(calculateGearRatio<double>(gears)*2025))};
}

vmc::Answer part2([[maybe_unused]] std::string_view input) {
    constexpr double requiredTurns = 10000000000000;
    auto gears = vmc::ints<double>(input);

    return {std::to_string(static_cast<long>(std::ceil(requiredTurns / calculateGearRatio<double>(gears))))};
}

vmc::Answer part3([[maybe_unused]] std::string_view input) {
    std::vector<std::pair<double,double>> gears{};

    for(const auto& line : vmc::lines(input)) {
        auto vals = vmc::ints<double>(line);
        if(!vals.empty()) {
            if(vals.size()==1) {
                gears.emplace_back(vals[0],vals[0]);
            } else {
                gears.emplace_back(vals.front(),vals.back());
            }
        } else {
            std::cout << "Parse Error: " << line << std::endl;
        }
    }
    return {std::to_string(static_cast<long>(calculatePairedGearRatio<double>(gears)*100))};
}

}  // namespace

VMC_QUEST("2025", 4, part1, part2, part3)
