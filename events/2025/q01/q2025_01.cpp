// Everybody Codes 2025 - Quest 1
// Approach write-up: NOTES.md in this folder.

#include "q2025_01.h"

namespace q2025_01 {

// Shared parsing and core logic. The three parts usually build on one idea
// and scale it up, so keep part1..3 thin and test the pieces from here.

}  // namespace q2025_01

namespace {

using namespace q2025_01;

vmc::Answer part1([[maybe_unused]] std::string_view input) {
    auto lines = vmc::split(input,"\n");
    auto names = vmc::split(lines[0],",");
    auto directions = vmc::split(lines[2],",");
    size_t index = 0;
    for(auto& d : directions) {
        int mult = d[0] == 'R' ? 1 : -1;
        int moves = vmc::toInt(d.substr(1));
        int i = static_cast<int>(index);
        i += moves*mult;
        if(i >= static_cast<int>(names.size())) {
            i = names.size()-1;
        } else if (i < 0) {
            i = 0;
        }
        index = static_cast<size_t>(i);
    }
    return {std::string(names[index])};
}

vmc::Answer part2([[maybe_unused]] std::string_view input) {
    auto lines = vmc::blocks(input);
    auto names = vmc::split(lines[0],",");
    auto directions = vmc::split(lines[1],",");
    size_t index = 0;
    for(auto& d : directions) {
        int mult = d[0] == 'R' ? 1 : -1;
        int moves = vmc::toInt(d.substr(1));
        int i = static_cast<int>(index);
        i += moves*mult;
        while(i < 0) {
            i += static_cast<int>(names.size());
        }
        while(i >= static_cast<int>(names.size())) {
            i -= static_cast<int>(names.size());
        }
        
        index = static_cast<size_t>(i);
    }
    return {std::string(names[index])};
}

vmc::Answer part3([[maybe_unused]] std::string_view input) {
    auto lines = vmc::blocks(input);
    auto names = vmc::split(lines[0],",");
    auto directions = vmc::split(lines[1],",");
    
    for(auto& d : directions) {
        size_t index = 0;
        int mult = d[0] == 'R' ? 1 : -1;
        int moves = vmc::toInt(d.substr(1));
        int i = static_cast<int>(index);
        i += moves*mult;
        while(i < 0) {
            i += static_cast<int>(names.size());
        }
        while(i >= static_cast<int>(names.size())) {
            i -= static_cast<int>(names.size());
        }
        std::swap(names[0],names[i]);
    }
    return {std::string(names[0])};
}

}  // namespace

VMC_QUEST("2025", 1, part1, part2, part3)
