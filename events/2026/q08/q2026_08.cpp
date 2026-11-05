// Everybody Codes 2026 - Quest 8
// Approach write-up: NOTES.md in this folder.

#include "q2026_08.h"
#include <vector>
#include <iostream>
namespace q2026_08 {

// Shared parsing and core logic. The three parts usually build on one idea
// and scale it up, so keep part1..3 thin and test the pieces from here.
//Generic Debug Functions
template <class T>
void printVector(const std::vector<T>& v) {
    std::cout << "v = {";
    auto x  = v.begin();
    if(x>= v.end()) {
        std::cout << "}";
    }
    while(x < v.end()) {
        std::cout << *x++ << (x==v.end() ? "}" : ",");
    }
    std::cout << std::endl;
}

std::vector<std::vector<int>> buildLines(std::string_view input, int direction = 0) {
    auto g = vmc::parseGrid(input,'.', direction % 2 == 1, direction > 2);
    // std::cout << vmc::toString(g);
    std::vector<std::vector<int>> entRows{};

    auto op = [&](const vmc::Point p){
        switch(direction%3) {
            case 0:
            case 1:
                return p+vmc::DOWN+vmc::RIGHT;
            
            case 2:
                return p+vmc::RIGHT;
        }
        return p;
    };

    for(int i = 0; direction%3 < 2 && i < g.cols();++i) {
        std::vector<int> vals{};
        vmc::Point p{0,i};
        while(g.get(p,'-') != '-') {
            if(g[p] != '.') {
                vals.push_back(static_cast<int>(g[p]-'0'));
            }
            p = op(p);
        }
        if(!vals.empty()) entRows.push_back(vals);
    }

    for(int i = direction%3 == 2 ? 0 : 1; i < g.cols();++i) {
        std::vector<int> vals{};
        vmc::Point p{i,0};
        while(g.get(p,'-') != '-') {
            if(g[p] != '.') {
                vals.push_back(static_cast<int>(g[p]-'0'));
            }
            p = op(p);
        }
        if(!vals.empty()) entRows.push_back(vals);
    }

    // for(const auto& v : entRows) {
    //     printVector(v);
    // }
    return entRows;
}

// std::vector<std::vector<int>> buildLines(std::string_view input) {
//     auto g = vmc::parseGrid(input,'.');
//     // std::cout << vmc::toString(g);
//     std::vector<std::vector<int>> entRows{};

//     for(int i = 0; i < g.cols();++i) {
//         std::vector<int> vals{};
//         for(int j = 0; i+j < g.rows() && j < g.cols();++j) {
//             vmc::Point p{i+j,j};
//             if(g[p] != '.') {
//                 vals.push_back(static_cast<int>(g[p]-'0'));
//             }
//         }
//         if(!vals.empty()) entRows.push_back(vals);
//     }

//         for(int i = 1; i < g.cols();++i) {
//         std::vector<int> vals{};
//         for(int j = 0; j < g.rows() && i+j < g.cols();++j) {
//             vmc::Point p{j,i+j};
//             if(g[p] != '.') {
//                 vals.push_back(static_cast<int>(g[p]-'0'));
//             }
//         }
//         if(!vals.empty()) entRows.push_back(vals);
//     }

//     // for(const auto& v : entRows) {
//     //     printVector(v);
//     // }
//     return entRows;
// }

int calcLineEnergy(const std::vector<int>& entLine) {
    int energy{0};
    int shadowHeight = 0;

    for(const auto val : entLine) {
        if(val > shadowHeight) {
            ++energy;
            shadowHeight = val;
        }
        --shadowHeight;
    }
    return energy;
}

std::pair<int,int> calcLineEnergy2(std::vector<int>& entLine) {
    int energy{0};
    int warriorCnt{0};
    int shadowHeight = 0;

    for(auto& val : entLine) {
        if(val > shadowHeight) {
            ++energy;
            shadowHeight = val;
            if(++val > 9) {
                val = -2;
                ++warriorCnt;
            }
        }
        if(val < 1) {
            ++val;
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
    auto ents = buildLines(input);
    auto energy = 0;
    for(const auto& el : ents) {
        energy += calcLineEnergy(el);
    }
    return {energy};
}

vmc::Answer part2(std::string_view input) {
    auto energy = 0;
    
    for(int i = 0; i < 6; ++i) {
        auto ents = buildLines(input,i);
    
        for(const auto& el : ents) {
            energy += calcLineEnergy(el);
        }
    }

    return {energy};
}

vmc::Answer part3([[maybe_unused]] std::string_view input) {
    auto energy = 0;
    auto warriors = 0;
    std::vector<std::vector<std::vector<int>>> entDirs;
    for(int i = 0; i < 6; ++i) {
        auto ents = buildLines(input,i);
        entDirs.push_back(ents);
    }
    int direction = 0;
    int rounds = 0;
    while(warriors < 100000) {
        for(auto& ents : entDirs[direction]) {
            auto [e,w] = calcLineEnergy2(ents);
            energy += e;
            warriors += w;
        }
        if(++direction >= 6) {
            direction = 0;
        }
        ++rounds;
    }

    return {rounds};
}

}  // namespace

VMC_QUEST("2026", 8, part1, part2, part3)
