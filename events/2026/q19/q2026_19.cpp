// Everybody Codes 2026 - Quest 19
// Approach write-up: NOTES.md in this folder.

#include "q2026_19.h"
#include <functional>
#include <numeric>
#include <unordered_map>

namespace q2026_19 {

// Shared parsing and core logic. The three parts usually build on one idea
// and scale it up, so keep part1..3 thin and test the pieces from here.
int64_t mixColors(std::string_view input, bool unknownBlock2 = false) {
    //Build the decoder ring
    const std::unordered_map<char, std::function<char (const char)>> decoderRing{
        {'C',[](const char c){switch(c) {case 'M':return 'B';case 'Y':return 'G';case 'K':return 'K';case 'C':default:return c;}}},
        {'M',[](const char c){switch(c) {case 'C':return 'B';case 'Y':return 'R';case 'K':return 'K';case 'M':default:return c;}}},
        {'Y',[](const char c){switch(c) {case 'M':return 'R';case 'C':return 'G';case 'K':return 'K';case 'Y':default:return c;}}},
        {'K',[](const char){return 'K';}}
    };

    //Overlay the windows
    std::vector<char> colors{'C','K','M','Y'};
    auto blcks = vmc::blocks(input);
    auto w1 = vmc::parseGrid(blcks.front());
    auto w2 = vmc::parseGrid(blcks.back());
    int64_t max = 0;
    do {
        std::unordered_map<char, int> charCount{};
        for(const auto p : w1.points()) {
            auto c1 = w1.get(p, '?');
            auto c2 = unknownBlock2 ? colors[w2.get(p, '?')-'1'] : w2.get(p, '?');
            ++charCount[decoderRing.at(c1)(c2)];
        }

        //Find highest char counts
        std::vector<int64_t> counts{};
        for(const auto& kvp : charCount) {
            // std::cout << kvp.first << ":" << kvp.second << std::endl;
            counts.push_back(kvp.second);
        }
        std::ranges::sort(counts,std::greater<>());
        if(counts.size() < 3) return 0;
        // printVector(counts);
        auto product = std::accumulate(counts.begin(),counts.begin()+3, static_cast<int64_t>(1), std::multiplies<>());
        if(product > max) max = product;
    }while(unknownBlock2 && std::next_permutation(colors.begin(),colors.end()));
    return max;
}

int64_t mixColorsOptimalOverlayWithNoBlack(std::string_view input) {
    //Overlay the windows
    auto blcks = vmc::blocks(input);
    auto w1 = vmc::parseGrid(blcks.front());
    // auto w2 = vmc::parseGrid(blcks.back()); // This is the same as w1
    //We want the colors to be the same count to maximize the product
    //Target would be number of points in the grid / 3
    //Since black can not be used, we'll have to take this into consideration
    std::unordered_map<char, size_t> charCount{};
    for(const auto p : w1.points()) {
        auto c1 = w1.get(p, '?');
        ++charCount[c1];
    }
    auto blackCount = charCount['K'];
    auto target = w1.points().size()/3;
    auto target_mod = w1.points().size()%3;
    int64_t max = 0;
    if(blackCount >= target) {
        //There's enough black so we need to use black and then divide the other two colors as even as possible
        target = (w1.points().size()-blackCount)/2;
        target_mod = (w1.points().size()-blackCount)%2;
        max = blackCount;
    } else {
        //Not a lot of black so can drop off those and then divide by 3
        target = (w1.points().size()-blackCount)/3;
        target_mod = (w1.points().size()-blackCount)%3;
        max = target + (target_mod > 0 ? 1 : 0);
    }
    if(target_mod > 0) --target_mod;
    max *= (target+(target_mod > 0 ? 1 : 0));
    if(target_mod > 0) --target_mod;
    max *= (target+(target_mod > 0 ? 1 : 0));
    return max;
}
}  // namespace q2026_19

namespace {

using namespace q2026_19;

vmc::Answer part1([[maybe_unused]] std::string_view input) {
    return {mixColors(input)};
}

vmc::Answer part2([[maybe_unused]] std::string_view input) {
    
    return {mixColors(input, true)};
}

vmc::Answer part3([[maybe_unused]] std::string_view input) {
    return {mixColorsOptimalOverlayWithNoBlack(input)};
}

}  // namespace

VMC_QUEST("2026", 19, part1, part2, part3)
