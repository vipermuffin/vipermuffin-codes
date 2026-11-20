// Everybody Codes 2026 - Quest 16
// Approach write-up: NOTES.md in this folder.

#include "q2026_16.h"
#include <unordered_map>
#include <string>

namespace q2026_16 {

// Shared parsing and core logic. The three parts usually build on one idea
// and scale it up, so keep part1..3 thin and test the pieces from here.
uint64_t rearrangeAndCountPasses(std::string_view input, const int numCopies = 1) {
    std::string orig(input);
    std::string shades(input);
    for(int i = 1; i < numCopies; ++i) {
        shades.append(orig);
    }
    uint64_t swaps{0};
    std::unordered_map<char, int> characters{};
    for(const auto& c : shades) {
        ++characters[c];
    }
    auto it = shades.begin();
    while(it != shades.end() && std::next(it) != shades.end() && *std::next(it) == *it) ++it;

    while(it != shades.end()) {
        auto lIt = shades.find_last_of(*it);
        if(lIt == std::string::npos || static_cast<long>(lIt) == std::distance(shades.begin(), it)) {
            //No more of these
            ++it;
        } else {
            std::reverse(std::next(it), std::next(shades.begin(),lIt+1));
            ++it;
            if(numCopies == 1) {
                ++swaps;
            } else {
                swaps += lIt - std::distance(shades.begin(), it)+1;
            }
        }
        while(it != shades.end() && std::next(it) != shades.end() && *std::next(it) == *it) ++it;
    }

    return swaps;
}
}  // namespace q2026_16

namespace {

using namespace q2026_16;

vmc::Answer part1([[maybe_unused]] std::string_view input) {
    return {rearrangeAndCountPasses(input)};
}

vmc::Answer part2([[maybe_unused]] std::string_view input) {
    return {rearrangeAndCountPasses(input, 10)};
}

vmc::Answer part3([[maybe_unused]] std::string_view input) {
    return {rearrangeAndCountPasses(input, 100)};
}

}  // namespace

VMC_QUEST("2026", 16, part1, part2, part3)
