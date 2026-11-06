// Everybody Codes 2026 - Quest 8
// Approach write-up: NOTES.md in this folder.

#include "q2026_08.h"
#include <array>
#include <iostream>
#include <ranges>
#include <functional>
#include <vector>
#include <unordered_map>
#include <format>

namespace q2026_08 {

// Shared parsing and core logic. The three parts usually build on one idea
// and scale it up, so keep part1..3 thin and test the pieces from here.
//Generic Debug Functions


std::vector<std::vector<vmc::Point>> buildLines(std::string_view input, std::unordered_map<vmc::Point, int>& entDb, int direction = 0) {
    auto g = vmc::parseGrid(input,'.', false, false);

    std::vector<std::vector<vmc::Point>> entRows{};
    auto rIdx = std::views::iota(0,g.rows());
    auto cIdx = std::views::iota(0,g.cols());
    std::array<std::function<int(int)>,6> rOp = {
        [](int i) {return i+1;},
        [](int i) {return i+1;},
        [](int i) {return i;},
        [&](int i) {return g.rows()-2-i;},
        [&](int i) {return g.rows()-2-i;},
        [&](int i) {return g.rows()-1-i;}
    };
    std::array<std::function<int(int)>,6> cOp = {
        [](int i) {return i;},
        [&](int i) {return g.cols()-1-i;},
        [&](int) {return g.cols();},
        [&](int i) {return g.cols()-1-i;},
        [](int i) {return i;},
        [&](int) {return g.cols();}
    };
    std::array<std::function<vmc::Point(vmc::Point)>,6> nextP = {
        [](vmc::Point pc) {return pc+vmc::DOWN+vmc::RIGHT;},
        [](vmc::Point pc) {return pc+vmc::DOWN+vmc::LEFT;},
        [](vmc::Point pc) {return pc+vmc::LEFT;}, 
        [](vmc::Point pc) {return pc+vmc::UP+vmc::LEFT;},
        [](vmc::Point pc) {return pc+vmc::UP+vmc::RIGHT;},
        [](vmc::Point pc) {return pc+vmc::RIGHT;},
    };

    std::array<int,6> rowVal = {0,0,0,g.rows()-1,g.rows()-1,g.rows()-1};
    std::array<int,6> colVal = {0,g.cols()-1,g.cols()-1,g.cols()-1,0,0};
    

    for(int i  : cIdx  | std::views::transform(cOp[direction])) {
        std::vector<vmc::Point> vals{};
        vmc::Point p{rowVal[direction],i};
        while(g.get(p,'-') != '-') {
            if(g[p] != '.') {
                vals.push_back(p);
                entDb[p] = static_cast<int>(g[p]-'0');
            }
            p = nextP[direction](p);
        }
        if(!vals.empty()) entRows.push_back(vals);
    }
    
    
    for(int i : rIdx | std::views::transform(rOp[direction])) {
        std::vector<vmc::Point> vals{};
        vmc::Point p{i,colVal[direction]};
        while(g.get(p,'-') != '-') {
            if(g[p] != '.') {
                vals.push_back(p);
                entDb[p] = static_cast<int>(g[p]-'0');
            }
            p = nextP[direction](p);
        }

        if(!vals.empty()) entRows.push_back(vals);
    }

    return entRows;
}

int calcLineEnergy(const std::vector<vmc::Point>& entLine, const std::unordered_map<vmc::Point, int>& entDb) {
    int energy{0};
    int shadowHeight = 0;

    for(const auto val : entLine) {
        if(entDb.at(val) > shadowHeight) {
            ++energy;
            shadowHeight = entDb.at(val);
        }
        --shadowHeight;
    }
    return energy;
}

std::pair<int,int> calcLineEnergy2(std::vector<vmc::Point>& entLine, std::unordered_map<vmc::Point, int>& entDb) {
    int energy{0};
    int warriorCnt{0};
    int shadowHeight = 0;

    for(auto& val : entLine) {
        if(entDb.at(val) < 1) {
            ++entDb[val];
        } else if(entDb.at(val) > shadowHeight) {
            ++energy;
            shadowHeight = entDb.at(val);
            if(++entDb[val] > 9) {
                entDb[val] = -2;
                ++warriorCnt;
            }
        }
        
        if(--shadowHeight < 0) {
            shadowHeight = 0;
        }
    }
    return std::make_pair(energy,warriorCnt);
}
}  // namespace q2026_08

namespace {

using namespace q2026_08;

vmc::Answer part1(std::string_view input) {
    std::unordered_map<vmc::Point, int> entDb{};
    auto ents = buildLines(input, entDb);
    auto energy = 0;
    for(const auto& el : ents) {
        energy += calcLineEnergy(el, entDb);
    }
    return {energy};
}

vmc::Answer part2(std::string_view input) {
    auto energy = 0;
    std::unordered_map<vmc::Point, int> entDb{};
    
    for(int i = 0; i < 6; ++i) {
        auto ents = buildLines(input,entDb,i);
    
        for(const auto& el : ents) {
            energy += calcLineEnergy(el, entDb);
        }
    }

    return {energy};
}

vmc::Answer part3([[maybe_unused]] std::string_view input) {
    auto energy = 0;
    auto warriors = 0;
    std::vector<std::vector<std::vector<vmc::Point>>> entDirs;
    std::unordered_map<vmc::Point, int> entDb{};
    for(int i = 0; i < 6; ++i) {
        auto ents = buildLines(input,entDb,i);
        entDirs.push_back(ents);
    }
    int direction = 0;
    while(warriors < 100000) {
        for(auto& ents : entDirs[direction]) {
            auto [e,w] = calcLineEnergy2(ents,entDb);
            energy += e;
            warriors += w;
        }
        if(++direction >= 6) {
            direction = 0;
        }
    }

    return {energy};
}

}  // namespace

VMC_QUEST("2026", 8, part1, part2, part3)
