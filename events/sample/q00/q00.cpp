// Sample quest showing the shape of a solution and the vmc helpers.
// Not a real Everybody Codes puzzle; delete the folder whenever you like.
//
// Part 1: sum every integer in the notes.
// Part 2: count '#' cells whose four neighbours are all '#'.
// Part 3: fewest steps from S to E, moving up/down/left/right, never onto '#'.

#include <deque>
#include <numeric>
#include <unordered_map>

#include "vmc/vmc.h"

namespace {

vmc::Answer part1(std::string_view input) {
    const auto nums = vmc::ints(input);
    return std::accumulate(nums.begin(), nums.end(), 0LL);
}

vmc::Answer part2(std::string_view input) {
    const auto grid = vmc::parseGrid(input);
    int count = 0;
    for (auto p : grid.findAll('#')) {
        bool interior = true;
        for (auto d : vmc::DIRS4) interior = interior && grid.get(p + d, '.') == '#';
        count += interior;
    }
    return count;
}

vmc::Answer part3(std::string_view input) {
    const auto grid = vmc::parseGrid(input);
    const auto start = grid.find('S');
    const auto end = grid.find('E');
    if (!start || !end) return "no S or E";

    std::unordered_map<vmc::Point, int> dist{{*start, 0}};
    std::deque<vmc::Point> queue{*start};
    while (!queue.empty()) {
        const auto p = queue.front();
        queue.pop_front();
        if (p == *end) return dist[p];
        for (auto n : grid.neighbours4(p)) {
            if (grid[n] == '#' || dist.contains(n)) continue;
            dist[n] = dist[p] + 1;
            queue.push_back(n);
        }
    }
    return "unreachable";
}

}  // namespace

VMC_QUEST("sample", 0, part1, part2, part3)
