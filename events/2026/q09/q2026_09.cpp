// Everybody Codes 2026 - Quest 9
// Approach write-up: NOTES.md in this folder.

#include "q2026_09.h"
#include <numeric>
#include <format>
#include <ranges>
#include <iostream>
#include <cstdint>
#include <map>
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
        // return std::format("Plant {},g{},l{}",id_,growth_,limit_);
        return std::format("id={}, growth={}, limit={}",id_,growth_,limit_);
    }
    int64_t id_;
    int64_t growth_;
    int64_t limit_;
};

int64_t simulateGrowth(const Plant& p, bool leafAlgorithm = false) {
    int64_t numBranches = 0;
    //Need to start at 1 in case the limit is 1 so I don't jump over
    int64_t simTime = 1;
    int64_t totalSize = 1;
    std::map<int64_t,int64_t> branches{{1,1}};
    int64_t step = leafAlgorithm ? p.growth_-1 : 1;
    while(totalSize < p.limit_) {
        std::map<int64_t,int64_t> newBranches{};
        int64_t nextStep = leafAlgorithm ? p.growth_/2+1 : 1;
        
        for(auto [sz,numB] : branches) {
            auto sizeB = sz+step;
            if(sizeB > p.growth_) {
                std::cout<< "Overshot: " << p.growth_ << "," << sizeB << std::endl;
            } else if(sizeB == p.growth_) {
                numBranches += numB;
                newBranches[sizeB/2] += numB*2;
                newBranches[1] += numB;
            } else {
                newBranches[sizeB] += numB;
                if((p.growth_-sizeB) < nextStep) {
                    nextStep = (p.growth_-sizeB);
                }
            }
        }
        if(leafAlgorithm) { 
            // totalSize = std::accumulate(newBranches.begin(),newBranches.end(),-numBranches, [](const auto sum, const auto kvp){return sum + kvp.second;});
            totalSize = numBranches+1;
        } else {
            totalSize = std::accumulate(newBranches.begin(),newBranches.end(),numBranches, [](const auto sum, const auto kvp){return sum + kvp.first*kvp.second;});
        }
        
        simTime += step;
        std::swap(branches,newBranches);
        std::swap(step,nextStep);
    }

    return simTime;
}

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

vmc::Answer part2([[maybe_unused]] std::string_view input) {
    int64_t total{0};
    for(auto line : vmc::lines(input)) {
        Plant p{line};
        auto t = simulateGrowth(p,true);
        total += t;
        // std::cout << p.str() << " " << t << " " << total << std::endl;
    }
    return {total};
}

vmc::Answer part3([[maybe_unused]] std::string_view input) {
    int64_t total{0};
    for(auto line : vmc::lines(input)) {
        Plant p{line};
        auto t = simulateGrowth(p,true);
        total += t;
        // std::cout << line << std::endl;
        // std::cout << p.str() << " " << t << " " << total << std::endl;
    }
    return {total};
}

// vmc::Answer part3_async([[maybe_unused]] std::string_view input) {
    
//     std::vector<std::future<int64_t>> futures;
//     for(auto line : vmc::lines(input)) {
//         Plant p{line};
//         futures.push_back(std::async(std::launch::async, simulateGrowth,p,true));
//     }

//     int64_t total{0};
//     for(auto& f : futures) {
//         total += f.get();
//     }

//     return {total};
// }

}  // namespace

VMC_QUEST("2026", 9, part1, part2, part3)
