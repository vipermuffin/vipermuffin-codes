// Everybody Codes 2026 - Quest 3
// Approach write-up: NOTES.md in this folder.

#include "q2026_03.h"
#include <ranges>

namespace q2026_03 {

// Shared parsing and core logic. The three parts usually build on one idea
// and scale it up, so keep part1..3 thin and test the pieces from here.
long convertVoteToDecimalNotation(const std::string_view votes) {
    auto vals = vmc::split(votes," ");
    long decimalVote{0};

    auto digitItr = vals.back().rbegin();
    auto multValItr = vals.front().rbegin();
    int mult = 0;
    while(digitItr != vals.back().rend()) {
        long digitVal = static_cast<long>(*digitItr - '0');
        int multVal = static_cast<int>(*multValItr - '0');
        
        for(int j = mult; j > 0; --j) {
            digitVal *= multVal;
        }
        decimalVote += digitVal;
        digitItr++;
        multValItr++;
        mult++;
        if(multValItr == vals.front().rend()) {
            multValItr = vals.front().rbegin();
        }
    }
    return decimalVote;
}

bool isValidIso2(const std::string_view votes) {
    auto decimalVoteVal = convertVoteToDecimalNotation(votes);
    auto input = vmc::ints<int>(votes);
    auto vals2Check = std::views::iota(1,input.back());
    for(const auto x : vals2Check) {
        auto dcv = convertVoteToDecimalNotation(std::format("{} {}",input.front(),x));
        if(dcv == decimalVoteVal) {
            return false;
        }
    }
    return true;
}
}  // namespace q2026_03

namespace {

using namespace q2026_03;

vmc::Answer part1(std::string_view input) {
    auto lines = vmc::lines(input);
    int totalDecimal{0};
    for(const auto line : lines) {
        totalDecimal += q2026_03::convertVoteToDecimalNotation(line.substr(4));
    }
    return {std::to_string(totalDecimal)};
}

vmc::Answer part2(std::string_view input) {
    auto lines = vmc::lines(input);
    long totalDecimal{0};
    for(const auto line : lines) {
        totalDecimal += q2026_03::convertVoteToDecimalNotation(line.substr(4));
    }
    return {std::to_string(totalDecimal)};
}

vmc::Answer part3(std::string_view input) {
    auto lines = vmc::lines(input);
    long totalDecimal{0};
    for(const auto line : lines) {
        if(isValidIso2(line.substr(5))) {
            totalDecimal += q2026_03::convertVoteToDecimalNotation(line.substr(5));
        }
    }
    return {std::to_string(totalDecimal)};
}

}  // namespace

VMC_QUEST("2026", 3, part1, part2, part3)
