// Everybody Codes 2026 - Quest 9
// Approach write-up: NOTES.md in this folder.

#include "q2026_09.h"
#include <numeric>
#include <format>
#include <ranges>
#include <iostream>
#include <cstdint>
#include <unordered_map>
#include <future>
#include <vector>

namespace q2026_09 {

// Shared parsing and core logic. The three parts usually build on one idea
// and scale it up, so keep part1..3 thin and test the pieces from here.
struct Plant {
    Plant(int64_t id, int64_t growth, int64_t limit):id_(id), growth_(growth), limit_(limit) {}
    Plant(std::string_view plantDef) {
        auto fields = vmc::split(plantDef, " ");
        id_ = vmc::ints<int64_t>(fields[0]).front();
        growth_ = vmc::ints<int64_t>(fields[1]).front();
        limit_ = vmc::ints<int64_t>(fields[2]).front();
    }
    std::string str() const {
        return std::format("Plant {},g{},l{}",id_,growth_,limit_);
    }
    int64_t id_;
    int64_t growth_;
    int64_t limit_;
};
int64_t simulateGrowth(const Plant& p, bool leafAlgorithm = false) {
    int64_t numBranches = 0;
    int64_t simTime = 0;
    int64_t totalSize = 0;
    std::unordered_map<int64_t,int64_t> branches{{0,1}};
    while(totalSize < p.limit_) {
        std::unordered_map<int64_t,int64_t> newBranches{};
        for(auto [sz,numB] : branches) {
            auto sizeB = sz;
            if(++sizeB == p.growth_) {
                numBranches += numB;
                newBranches[sizeB/2] += numB*2;
                newBranches[1] += numB;
            } else {
                newBranches[sizeB] += numB;
            }
        }
        if(leafAlgorithm) { 
            totalSize = std::accumulate(newBranches.begin(),newBranches.end(),-numBranches, [](const auto sum, const auto kvp){return sum + kvp.second;});
        } else {
            totalSize = std::accumulate(newBranches.begin(),newBranches.end(),numBranches, [](const auto sum, const auto kvp){return sum + kvp.first*kvp.second;});
        }
        
        ++simTime;
        // std::cout << std::format("Second {}:Size {}\n",simTime,totalSize);
        std::swap(branches,newBranches);
    }

    return simTime;
}
// int64_t simulateGrowth(const Plant& p, bool leafAlgorithm = false) {
//     int64_t numBranches = 0;
//     int64_t simTime = 0;
//     int64_t totalSize = 0;
//     std::vector<int64_t> branches{0};
//     while(totalSize < p.limit_) {
//         std::vector<int64_t> newBranches{};
//         newBranches.reserve(branches.size()*2);
//         for(auto b : branches) {
//             if(++b == p.growth_) {
//                 ++numBranches;
//                 newBranches.push_back(b/2);
//                 newBranches.push_back(b/2);
//                 newBranches.push_back(1);
//             } else {
//                 newBranches.push_back(b);
//             }
//         }
//         if(leafAlgorithm) { 
//             totalSize = newBranches.size() - numBranches;
//         } else {
//             totalSize = std::accumulate(newBranches.begin(),newBranches.end(),numBranches);
//         }
        
//         ++simTime;
//         std::swap(branches,newBranches);
//     }

//     return simTime;
// }
}  // namespace q2026_09

namespace {

using namespace q2026_09;

vmc::Answer part1(std::string_view input) {
    int64_t total{0};
    for(auto line : vmc::lines(input)) {
        Plant p{line};
        auto t = simulateGrowth(p);
        total += t;
        // std::cout << p.str() << " " << t << " " << total << std::endl;
    }
    return {total};
}

// vmc::Answer part2([[maybe_unused]] std::string_view input) {
//     int64_t total{0};
//     for(auto line : vmc::lines(input)) {
//         Plant p{line};
//         auto t = simulateGrowth(p,true);
//         total += t;
//         // std::cout << p.str() << " " << t << " " << total << std::endl;
//     }
//     return {total};
// }

// vmc::Answer part3([[maybe_unused]] std::string_view input) {
//     int64_t total{0};
//     for(auto line : vmc::lines(input)) {
//         Plant p{line};
//         auto t = simulateGrowth(p,true);
//         total += t;
//         // std::cout << p.str() << " " << t << " " << total << std::endl;
//     }
//     return {total};
// }

vmc::Answer part2([[maybe_unused]] std::string_view input) {
    
    std::vector<std::future<int64_t>> futures;
    for(auto line : vmc::lines(input)) {
        Plant p{line};
        futures.push_back(std::async(std::launch::async, simulateGrowth,p,true));
    }

    int64_t total{0};
    for(auto& f : futures) {
        total += f.get();
    }

    return {total};
}

vmc::Answer part3([[maybe_unused]] std::string_view input) {
    
    std::vector<std::future<int64_t>> futures;
    for(auto line : vmc::lines(input)) {
        Plant p{line};
        futures.push_back(std::async(std::launch::async, simulateGrowth,p,true));
    }

    int64_t total{0};
    for(auto& f : futures) {
        total += f.get();
    }

    return {total};
}

}  // namespace

VMC_QUEST("2026", 9, part1, part2, part3)
