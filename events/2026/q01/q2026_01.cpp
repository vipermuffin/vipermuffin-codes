// Everybody Codes 2026 - Quest 1
// Approach write-up: NOTES.md in this folder.

#include "q2026_01.h"
#include <algorithm>
#include <ranges>
#include <string>

namespace q2026_01 {

// Shared parsing and core logic. The three parts usually build on one idea
// and scale it up, so keep part1..3 thin and test the pieces from here.

}  // namespace q2026_01

namespace {

using namespace q2026_01;

vmc::Answer part1(std::string_view input) {
    const auto outpost_cones = vmc::ints<int>(input);
    const auto required_cones = std::views::iota(1,std::ranges::max(outpost_cones)+1)
                        | std::views::transform([](int i) {return i*i;});

    return std::to_string(total_shortfall(outpost_cones, required_cones));
}

vmc::Answer part2(std::string_view input) {
    const auto outpost_cones = vmc::ints<int>(input);
    const auto required_cones = std::views::iota(3,std::ranges::max(outpost_cones)*4+4)
                        | std::views::transform([](int i) {return (i*i)-((i-2)*(i-2));});

    return std::to_string(total_shortfall(outpost_cones, required_cones));
}

vmc::Answer part3(std::string_view input) {
    const auto outpost_cones = vmc::ints<long>(input);
    const auto required_cones = std::views::iota(1L,std::ranges::max(outpost_cones))
                        | std::views::transform([](long x) {return 2*x+1;})
                        | std::views::transform([](long i) {return i*i-1;});

    return std::to_string(total_shortfall(outpost_cones, required_cones));
}
}  // namespace

VMC_QUEST("2026", 1, part1, part2, part3)
