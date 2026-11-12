// Everybody Codes 2026 - Quest 11
// Approach write-up: NOTES.md in this folder.

#include "q2026_11.h"
#include <algorithm>
#include <numeric>
#include <format>
#include <ranges>
#include <unordered_map>
#include <unordered_set>
#include <queue>


namespace q2026_11 {

// Shared parsing and core logic. The three parts usually build on one idea
// and scale it up, so keep part1..3 thin and test the pieces from here.
using ConnectionMap = std::unordered_map<int, std::unordered_set<int>>;
using PortalMap = std::unordered_map<int, std::unordered_map<int, int>>;

ConnectionMap buildDefaultPortalConnections(int numPortals, int start = 1, bool use3 = false) {
    ConnectionMap connections{};
    auto fullNumRange = std::views::iota(start,numPortals+1);
    for(auto i : fullNumRange) {
        for(auto x = (use3 ? -3 : -1) ; x < (use3 ? 4 : 2); ++x) {
            if(x != 0) {
                auto nxIdx = i+x;
                if(nxIdx > numPortals) {
                    nxIdx -= numPortals;
                } if(nxIdx < start) {
                    nxIdx += numPortals;
                }
                connections[i].insert(nxIdx);
            }
        }
    }
    return connections;
}

ConnectionMap updateConnectionsWithShortcuts(const std::vector<std::pair<int,int>>& shortcuts, const ConnectionMap& cMap) {
    ConnectionMap connections{cMap};
    for(const auto& pVals : shortcuts) {
        connections[pVals.first].insert(pVals.second);
        connections[pVals.second].insert(pVals.first);
    }
    return connections;
}

// int findPath(int steps, const int start, const int finish, const ConnectionMap& connections, std::unordered_set<int> visited) {
//     if(connections.at(start).contains(finish)) {
//         return steps + 1;
//     }
//     int minPath = INT_MAX;
//     for(const auto x : connections.at(start)) {
//         if(!visited.contains(x)) {
//             visited.insert(x);
//             auto val = findPath(steps+1, x, finish, connections, visited);
//             if(val < minPath) {
//                 minPath = val;
//             }
//         }
//     }

//     return minPath;
// }

// int findShortestPath(const int start, const int finish, const ConnectionMap& connections) {
//     std::unordered_set<int> visited{start};
//     return findPath(0, start, finish, connections, visited);
// }

int findShortestPath_BFS(const int start, const int finish, const ConnectionMap& connections) {
    std::unordered_set<int> visited{};
    std::queue<std::pair<int,int>> nodesWithSteps{};
    nodesWithSteps.push( std::make_pair(start,0));
    //search for finish
    while(!nodesWithSteps.empty()) {
        auto [currentNode, currentSteps] = nodesWithSteps.front();
        nodesWithSteps.pop();
        if(currentNode == finish) {
            return 1 + currentSteps;
        } else if( !visited.contains(currentNode)){
            visited.insert(currentNode);
            for(auto& n : connections.at(currentNode)) {
                nodesWithSteps.emplace(n,currentSteps+1);
            }
        }
    }

    return INT_MAX;
}

int calculateStrain(const ConnectionMap& connections) {
    auto strain = 0;
    auto numPortals = std::ranges::max_element(connections,{},[](const auto& kvp){return kvp.first;})->first;
    PortalMap pm{};
    for(const auto& port : connections) {
        for(auto x : std::views::iota(1,numPortals+1) | std::views::filter([&](auto i){return i != port.first;})) {
            int val = 0;
            if(pm.contains(port.first) && pm[port.first].contains(x)) {
                val = pm[port.first][x];
            } else {
                val = findShortestPath_BFS(port.first, x, connections);
                pm[port.first][x] = val;
                pm[x][port.first] = val;
            }
            strain += val;
        }
    }
    return strain;
}
}  // namespace q2026_11

namespace {

using namespace q2026_11;

vmc::Answer part1([[maybe_unused]] std::string_view input) {
    auto sections = vmc::blocks(input);
    auto numPorts = vmc::ints<int>(sections.front()).front();
    auto cMap = buildDefaultPortalConnections(numPorts);

    auto defaultStrain = calculateStrain(cMap);

    std::vector<std::pair<int,int>> shortcuts{};
    for(auto& line : vmc::lines(sections.back())) {
        auto scVals = vmc::ints<int>(line);
        shortcuts.emplace_back(scVals.front(),scVals.back());
    }
    auto cnMap = updateConnectionsWithShortcuts(shortcuts, cMap);
    auto updatedStrain = calculateStrain(cnMap);

    return {defaultStrain-updatedStrain};
}

vmc::Answer part2([[maybe_unused]] std::string_view input) {
    auto sections = vmc::blocks(input);
    auto numPorts = vmc::ints<int>(sections.front()).front();
    auto cMap = buildDefaultPortalConnections(numPorts, 1, true);

    auto defaultStrain = calculateStrain(cMap);

    std::vector<std::pair<int,int>> shortcuts{};
    for(auto& line : vmc::lines(sections.back())) {
        auto scVals = vmc::ints<int>(line);
        shortcuts.emplace_back(scVals.front(),scVals.back());
    }
    auto cnMap = updateConnectionsWithShortcuts(shortcuts, cMap);
    auto updatedStrain = calculateStrain(cnMap);

    for(const auto& id: std::views::iota(1,(numPorts)+1)) {
        for(auto i : std::views::iota(1,numPorts+1) | std::views::filter([&](auto z){return z != id;})) {
            shortcuts.emplace_back(id, i);
            cnMap = updateConnectionsWithShortcuts(shortcuts, cMap);
            shortcuts.pop_back();
            
            auto newStrain = calculateStrain(cnMap);
            if(newStrain < updatedStrain) {
                updatedStrain = newStrain;
            }
        }
    }

    return {defaultStrain-updatedStrain};
}

vmc::Answer part3([[maybe_unused]] std::string_view input) {
    auto sections = vmc::blocks(input);
    auto numPorts = vmc::ints<int>(sections.front()).front();
    auto cMap = buildDefaultPortalConnections(numPorts, 1, true);

    auto defaultStrain = calculateStrain(cMap);

    std::vector<std::pair<int,int>> shortcuts{};
    for(auto& line : vmc::lines(sections.back())) {
        auto scVals = vmc::ints<int>(line);
        shortcuts.emplace_back(scVals.front(),scVals.back());
    }
    auto cnMap = updateConnectionsWithShortcuts(shortcuts, cMap);
    auto updatedStrain = calculateStrain(cnMap);

    for(const auto& id: std::views::iota(1,(numPorts/8)+1)) {
        auto spacing = numPorts/8;
        std::vector<int> crystals{id,(id+spacing),(id+spacing*2),(id+spacing*3),(id+spacing*4),(id+spacing*5),(id+spacing*6),(id+spacing*7)};
        constexpr int scaledVal = 100;
        std::unordered_set<std::string> paired{};
        do {
            auto p1Val = crystals[0] < crystals[1] ? crystals[0]*scaledVal + crystals[1] : crystals[1]*scaledVal + crystals[0];
            auto p2Val = crystals[2] < crystals[3] ? crystals[2]*scaledVal + crystals[3] : crystals[3]*scaledVal + crystals[2];
            auto p3Val = crystals[4] < crystals[5] ? crystals[4]*scaledVal + crystals[5] : crystals[5]*scaledVal + crystals[4];
            auto p4Val = crystals[6] < crystals[7] ? crystals[6]*scaledVal + crystals[7] : crystals[7]*scaledVal + crystals[6];
            auto pairStr = std::format("{},{},{},{}",p1Val,p2Val,p3Val,p4Val);
            if(paired.contains(pairStr)) {
                // already done
            } else {
                paired.insert(pairStr);
                shortcuts.emplace_back(crystals[0], crystals[1]);
                shortcuts.emplace_back(crystals[2], crystals[3]);
                shortcuts.emplace_back(crystals[4], crystals[5]);
                shortcuts.emplace_back(crystals[6], crystals[7]);
                
                cnMap = updateConnectionsWithShortcuts(shortcuts, cMap);                
                auto newStrain = calculateStrain(cnMap);
                if(newStrain < updatedStrain) {
                    updatedStrain = newStrain;
                }
                
                shortcuts.pop_back();
                shortcuts.pop_back();
                shortcuts.pop_back();
                shortcuts.pop_back();
            }
        }while(std::next_permutation(crystals.begin(), crystals.end()));

    }

    return {defaultStrain-updatedStrain};
}

}  // namespace

VMC_QUEST("2026", 11, part1, part2, part3)
