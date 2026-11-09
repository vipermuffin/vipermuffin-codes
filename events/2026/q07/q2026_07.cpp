// Everybody Codes 2026 - Quest 7
// Approach write-up: NOTES.md in this folder.

#include "q2026_07.h"
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <numeric>
#include <format>
#include <iostream>
namespace q2026_07 {

// Shared parsing and core logic. The three parts usually build on one idea
// and scale it up, so keep part1..3 thin and test the pieces from here.
std::pair<std::unordered_map<int,int>, std::unordered_map<int,std::vector<int>>> buildCloudMap(std::string_view input) {
    std::unordered_map<int,int> cloudMap{};
    std::unordered_map<int,std::vector<int>> allConnections{};

    // std::cout << input << std::endl;
    for(auto line : vmc::lines(input)) {
        auto fields = vmc::split(line, " ");
        auto id = vmc::ints<int>(fields[0]).front();
        auto energy = vmc::ints<int>(fields[1]).front();
        auto connections = vmc::ints<int>(fields[2]);
        cloudMap[id] = energy;
        allConnections.emplace(id,connections);
    }
    return std::make_pair(cloudMap,allConnections);
}

void performImpulse(std::unordered_map<int,int>& clouds, std::unordered_map<int,std::vector<int>>& connections, int impulse) {
    auto startCloud = std::ranges::max_element(connections,{},[](const auto x){return x.second.size();});
    std::vector<int> stack{};
    stack.push_back(startCloud->first);
    while(impulse > 0) {
        std::vector<int> nextStack{};
        nextStack.reserve(startCloud->second.size()*stack.size());
        for(const auto cId : stack) {
            clouds[cId] += impulse;
            for( const auto nxId : connections[cId]) {
                nextStack.push_back(nxId);
            }
        }
        --impulse;
        std::swap(stack,nextStack);
    }
}

void performExplodingImpulse(std::unordered_map<int,int>& clouds, std::unordered_map<int,std::vector<int>>& connections, int impulse, int startId) {
    constexpr int ExplodingVal = 1000000;
    constexpr int ExplodingEnergy = 10;
    auto startCloud = std::ranges::max_element(connections,{},[](const auto x){return x.second.size();});
    std::vector<std::pair<int,int>> stack{};
    stack.emplace_back(startId,impulse);
    while(!stack.empty()) {
        std::vector<std::pair<int,int>> nextStack{};
        nextStack.reserve(startCloud->second.size()*stack.size());
        std::unordered_set<int> explodingStack{};
        explodingStack.reserve(stack.size());

        //Normal impulses
        for(const auto [cId, cImp] : stack) {
            
            if(clouds.contains(cId)) {
                // std::cout << "cloud " << cId << " received 1 impulse with strength " << cImp << " (energy " << clouds[cId] << " -> " << cImp + clouds[cId] << ")" << std::endl;
                clouds[cId] += cImp;
                if(cImp > 1) {
                    for( const auto nxId : connections[cId]) {
                        nextStack.emplace_back(nxId,cImp-1);
                    }
                }
                if(clouds[cId] >= ExplodingVal) {
                    explodingStack.insert(cId);
                }
            }
        }
        
        //Exploding impluses
        for(const auto eId : explodingStack) {
            // std::cout << "cloud " << eId << " exploded at energy " << clouds[eId] << std::endl;
            for(const auto nxId : connections[eId]) {
                if(clouds.contains(nxId)) nextStack.emplace_back(nxId,ExplodingEnergy);
            }
            clouds.erase(eId);
            connections.erase(eId);
        }
        std::swap(stack,nextStack);
    }
}
}  // namespace q2026_07

namespace {

using namespace q2026_07;

vmc::Answer part1([[maybe_unused]] std::string_view input) {
    auto [clouds,connections] = buildCloudMap(input);
    performImpulse(clouds, connections, 5);
    return {std::accumulate(clouds.begin(),clouds.end(),0, [](const auto sum, const auto kvp) {return sum + kvp.second;})};
}

vmc::Answer part2([[maybe_unused]] std::string_view input) {
    auto [clouds,connections] = buildCloudMap(input);
    performImpulse(clouds, connections, 10);
    return {std::accumulate(clouds.begin(),clouds.end(),0, [](const auto sum, const auto kvp) {return sum + kvp.second;})};
}

vmc::Answer part3([[maybe_unused]] std::string_view input) {
    auto [clouds,connections] = buildCloudMap(input);
    auto maxEnergy = 0L;
    for(const auto cloud : clouds) {
        auto cloudsCpy{clouds};
        auto connectionsCpy{connections};
        // std::cout << "===================Initial Impluse Cloud " << cloud.first << "===============================\n";
        performExplodingImpulse(cloudsCpy, connectionsCpy, 10, cloud.first);
        auto totalEnergy = std::accumulate(cloudsCpy.begin(),cloudsCpy.end(),0L, [](const auto sum, const auto kvp) {return sum + kvp.second;});
        // std::cout << cloud.first << ":" << totalEnergy << std::endl;
        // std::cout << "========================================================================\n";
        if(totalEnergy > maxEnergy) {
            maxEnergy = totalEnergy;
        }
    }
    
    return {maxEnergy};
}

}  // namespace

VMC_QUEST("2026", 7, part1, part2, part3)
