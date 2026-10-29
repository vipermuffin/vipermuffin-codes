// Everybody Codes 2026 - Quest 3
// Approach write-up: NOTES.md in this folder.

#include "q2026_03.h"
#include <ranges>

namespace q2026_03 {

// Shared parsing and core logic. The three parts usually build on one idea
// and scale it up, so keep part1..3 thin and test the pieces from here.
std::int64_t convertVoteToDecimalNotation(const std::string_view stdNum, const std::string_view votes) {
    // auto vals = vmc::split(votes," ");
    std::int64_t decimalVote{0};

    auto digitItr = votes.rbegin();
    auto multValItr = stdNum.rbegin();
    int mult = 0;
    while(digitItr != votes.rend()) {
        std::int64_t digitVal = static_cast<std::int64_t>(*digitItr - '0');
        int multVal = static_cast<int>(*multValItr - '0');
        
        std::ranges::for_each(std::views::iota(0,mult),
        [&](const int){digitVal *= multVal;});
        decimalVote += digitVal;
        ++digitItr;
        ++mult;
        if(++multValItr == stdNum.rend()) {
            multValItr = stdNum.rbegin();
        }
    }
    return decimalVote;
}

bool isValidIso2(const std::string_view votes) {
    auto vals = vmc::split(votes," ");
    auto decimalVoteVal = convertVoteToDecimalNotation(vals.front(),vals.back());
    auto input = vmc::ints<int>(votes);
    return std::ranges::none_of(std::views::iota(1,input.back()),
    [&](const int x ){
        return convertVoteToDecimalNotation(vals.front(),std::to_string(x)) == decimalVoteVal;
    });
}
}  // namespace q2026_03

namespace {

using namespace q2026_03;

vmc::Answer part1(std::string_view input) {
    auto lines = vmc::lines(input);
    std::int64_t totalDecimal{0};
    for(const auto line : lines) {
        auto vals = vmc::split(line.substr(4)," ");
        totalDecimal += q2026_03::convertVoteToDecimalNotation(vals.front(),vals.back());
    }
    return {totalDecimal};
}

vmc::Answer part2(std::string_view input) {
    auto lines = vmc::lines(input);
    std::int64_t totalDecimal{0};
    for(const auto line : lines) {
        auto vals = vmc::split(line.substr(4)," ");
        totalDecimal += q2026_03::convertVoteToDecimalNotation(vals.front(),vals.back());
    }
    return {totalDecimal};
}

vmc::Answer part3(std::string_view input) {
    auto lines = vmc::lines(input);
    std::int64_t totalDecimal{0};
    for(const auto line : lines) {
        if(isValidIso2(line.substr(5))) {
            auto vals = vmc::split(line.substr(5)," ");
            totalDecimal += q2026_03::convertVoteToDecimalNotation(vals.front(),vals.back());
        }
    }
    return {totalDecimal};
}

}  // namespace

VMC_QUEST("2026", 3, part1, part2, part3)
