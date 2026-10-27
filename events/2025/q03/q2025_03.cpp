// Everybody Codes 2025 - Quest 3
// Approach write-up: NOTES.md in this folder.

#include "q2025_03.h"
#include <algorithm>

namespace q2025_03 {

// Shared parsing and core logic. The three parts usually build on one idea
// and scale it up, so keep part1..3 thin and test the pieces from here.

}  // namespace q2025_03

namespace {

using namespace q2025_03;

vmc::Answer part1([[maybe_unused]] std::string_view input) {
    auto boxSizes = vmc::ints<int>(input);
    std::sort(boxSizes.rbegin(), boxSizes.rend());
    auto frontItr = boxSizes.begin();
    int count = 0;
    int lastVal = INT_MAX;
    while(frontItr != boxSizes.end())  {
        if(*frontItr < lastVal) {
            count += *frontItr;
        }
        lastVal = *frontItr;
        frontItr = std::next(frontItr);
    }
    return {std::to_string(count)};
}

vmc::Answer part2([[maybe_unused]] std::string_view input) {
    constexpr int mushroom_sz = 20;

    auto boxSizes = vmc::ints<int>(input);
    std::sort(boxSizes.begin(), boxSizes.end());
    auto frontItr = boxSizes.begin();
    int count = 0;
    int crates = 0;
    int lastVal = INT_MIN;
    while(crates != mushroom_sz && frontItr != boxSizes.end())  {
        if(*frontItr > lastVal) {
            count += *frontItr;
            crates++;
        }
        lastVal = *frontItr;
        frontItr = std::next(frontItr);
    }
    return {std::to_string(count)};
}

vmc::Answer part3([[maybe_unused]] std::string_view input) {
    auto boxSizes = vmc::ints<int>(input);
    std::sort(boxSizes.rbegin(), boxSizes.rend());
    int count = 0;
    while(!boxSizes.empty()) {
        auto frontItr = boxSizes.begin();
        int lastVal = INT_MAX;
        std::vector<int> next{};
        while(frontItr != boxSizes.end())  {
            if(*frontItr == lastVal) {
                next.push_back(*frontItr);
            }
            lastVal = *frontItr;
            frontItr = std::next(frontItr);
        }
        count++;
        std::swap(next,boxSizes);
    }

    return {std::to_string(count)};
}

}  // namespace

VMC_QUEST("2025", 3, part1, part2, part3)
