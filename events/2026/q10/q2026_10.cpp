// Everybody Codes 2026 - Quest 10
// Approach write-up: NOTES.md in this folder.

#include "q2026_10.h"
#include <algorithm>
#include <cstdint>
#include <format>
#include <ranges>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace q2026_10 {

// Shared parsing and core logic. The three parts usually build on one idea
// and scale it up, so keep part1..3 thin and test the pieces from here.
struct Ghost {
    Ghost(const vmc::Point& loc, const vmc::Point& dir, bool isStrong = false) : currentLocation_(loc), currentDirection_(dir), collisionCount_(0), strong_(isStrong) {}
    vmc::Point nextLocation() const {return currentLocation_+currentDirection_;}
    bool isGone() const {return collisionCount_ >= (strong_ ? strongResistance : resistance);}
    std::string str() const {return std::format("({},{},{},{})",currentLocation_.c,currentLocation_.r,currentDirection_.c,currentDirection_.r);}
    vmc::Point currentLocation_;
    vmc::Point currentDirection_;
    int64_t collisionCount_;
    bool strong_;

    const int64_t resistance = 10000;
    const int64_t strongResistance = 1000000000000;
};

std::pair<std::unordered_set<vmc::Point>,std::vector<Ghost>> buildWallGhostMap(const std::string_view input) {
    std::unordered_set<vmc::Point> wallLocs{};
    std::vector<Ghost> ghosts{};
    auto g = vmc::parseGrid(input);
    for(const auto& p : g.points()) {
        auto c = g[p];
        if(c == '#') {
            wallLocs.insert(p);
        } else if (c == '>') {
            ghosts.emplace_back(p,vmc::RIGHT);
        } else if (c == '<') {
            ghosts.emplace_back(p,vmc::LEFT);
        } else if (c == '^') {
            ghosts.emplace_back(p,vmc::UP);
        } else if (c == 'v') {
            ghosts.emplace_back(p,vmc::DOWN);
        } else {
            //Nothing needed
        }
    }
    return std::make_pair(wallLocs,ghosts);
}

int moveGhosts(std::unordered_set<vmc::Point>& wallLocations,std::vector<Ghost>& ghosts) {
    int collisionCount = 0;
    std::unordered_map<vmc::Point, std::vector<std::reference_wrapper<Ghost>>> ghostCollideTracker;
    for(auto& ghost : ghosts) {
        if(!ghost.isGone()) {
            auto nextP = ghost.nextLocation();
            if(wallLocations.contains(nextP)) {
                //Hit wall, don't move forward but turn clockwise
                ghost.currentDirection_ = ghost.currentDirection_.turnRight();
                ++ghost.collisionCount_;
                ++collisionCount;
            } else {
                ghost.currentLocation_ = nextP;
            }
            if(!ghost.isGone()) ghostCollideTracker[ghost.currentLocation_].push_back(ghost);
        }
    }

    for(auto [loc,gv] : ghostCollideTracker) {
        if(gv.size() > 1) {
            for(auto g : gv) {
                auto& ghost = g.get();
                //collide plus turn 180
                ++ghost.collisionCount_;
                ghost.currentDirection_ = ghost.currentDirection_.turnRight();
                ghost.currentDirection_ = ghost.currentDirection_.turnRight();
                ++collisionCount;
            }
        }
    }
    return collisionCount;
}

std::string makeGhostVectorStr(const std::vector<Ghost>& gv) {
    std::stringstream ss{};
    for(const auto& g : gv) {
        ss << g.str();
    }
    return ss.str();
}

}  // namespace q2026_10

namespace {

using namespace q2026_10;

vmc::Answer part1(std::string_view input) {
    constexpr int ROUNDS = 10;
    auto [walls,ghosts] = buildWallGhostMap(input);
    auto collisions = 0;
    for(auto i = 0; i < ROUNDS; ++i) {
        auto collides = moveGhosts(walls, ghosts);
        collisions += collides;
    }
    
    return {collisions};
}

vmc::Answer part2(std::string_view input) {
    int64_t rounds = 0;
    auto [walls,ghosts] = buildWallGhostMap(input);
    [[maybe_unused]] auto collisions = 0;
    while(std::ranges::count_if(ghosts,[](const auto& g) {return !g.isGone();})>0) {
        ++rounds;
        auto collides = moveGhosts(walls, ghosts);
        collisions += collides;
    }
    
    return {rounds};
}

vmc::Answer part3(std::string_view input) {
    int64_t rounds = 0;
    auto [walls,ghosts] = buildWallGhostMap(input);
    std::ranges::for_each(ghosts,[](auto& g) {g.strong_=true;});
    std::unordered_map<std::string, std::pair<int64_t,std::vector<Ghost>>> seen{};
    auto remaining = ghosts.size();
    while(remaining > 0) {
        auto prevRemaining = remaining;
        ++rounds;
        [[maybe_unused]] auto collides = moveGhosts(walls, ghosts);
        auto gvId = makeGhostVectorStr(ghosts);
        auto [it, inserted] = seen.try_emplace(gvId,std::make_pair(rounds,ghosts));
        if(!inserted) {
            //we already have seen this formation
            //need to find the biggest difference so we don't over shoot
            int64_t mult = INT64_MAX;
            
            for(size_t i = 0; i < ghosts.size(); ++i) {
                if(!ghosts[i].isGone()) {
                    auto iDiff = ghosts[i].collisionCount_ - it->second.second[i].collisionCount_;
                    auto iMult = (ghosts[i].strongResistance - ghosts[i].collisionCount_) / iDiff;
                    if(iMult < mult) {
                        mult = iMult;
                    }
                }
            }
            
            if(mult >= 1) --mult;  // go one chunk less to underdamp the step
            for(size_t i = 0; i < ghosts.size(); ++i) {
                if(!ghosts[i].isGone()) {
                    auto diff = ghosts[i].collisionCount_ - it->second.second[i].collisionCount_;
                    ghosts[i].collisionCount_+=mult*diff;
                }
            }
            auto diff = rounds - it->second.first;
            rounds += mult*diff;
            seen.clear();
            seen.emplace(gvId,std::make_pair(rounds,ghosts));
        }
        remaining = std::ranges::count_if(ghosts,[](const auto& g) {return !g.isGone();});
        if(prevRemaining != remaining) {
            seen.clear();
        }
    }
    
    return {rounds};
}

}  // namespace

VMC_QUEST("2026", 10, part1, part2, part3)
