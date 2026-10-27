#include <array>
#include <ranges>
#include <stdexcept>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "q2026_01.h"

namespace {

constexpr const char* kEvent = "2026";
constexpr int kQuest = 1;

// Skips until the expected answer is filled in.
void checkExample(int part, std::string_view example, std::string expected) {
    if (expected.empty()) GTEST_SKIP() << "expected answer for part " << part << " not filled in yet";
    EXPECT_EQ(vmc::solve(kEvent, kQuest, part, example), vmc::Answer(expected));
}

// --- Unit tests for the pieces declared in q2026_01.h ------------------------

TEST(Event2026Quest01, TotalShortfallEmptyStock) {
    const std::vector<int> stock{};
    const std::vector<int> thresholds{1, 4, 9};
    EXPECT_EQ(q2026_01::total_shortfall(stock, thresholds), 0);
}

TEST(Event2026Quest01, TotalShortfallExactMatchesNeedNothing) {
    const std::vector<int> stock{1, 4, 9};
    const std::vector<int> thresholds{1, 4, 9};
    EXPECT_EQ(q2026_01::total_shortfall(stock, thresholds), 0);
}

TEST(Event2026Quest01, TotalShortfallVectorThresholds) {
    // Part 1 example with the squares written out by hand: 1+5+4+0+1+0
    const std::vector<int> stock{3, 11, 12, 16, 24, 25};
    const std::vector<int> thresholds{1, 4, 9, 16, 25, 36};
    EXPECT_EQ(q2026_01::total_shortfall(stock, thresholds), 11);
}

TEST(Event2026Quest01, TotalShortfallLazyViewThresholds) {
    // Same as above, but the thresholds are never stored anywhere.
    const std::vector<int> stock{3, 11, 12, 16, 24, 25};
    const auto squares = std::views::iota(1, 64) | std::views::transform([](int i) { return i * i; });
    static_assert(std::ranges::random_access_range<decltype(squares)>);
    EXPECT_EQ(q2026_01::total_shortfall(stock, squares), 11);
}

TEST(Event2026Quest01, TotalShortfallOddSquaresMinusOne) {
    // Part 3 example: thresholds 8, 24, 48, 80, ... -> 5+13+12+8+0+23
    const std::vector<long> stock{3, 11, 12, 16, 24, 25};
    const auto thresholds = std::views::iota(1L, 1750L)
                          | std::views::transform([](long k) { return (2 * k + 1) * (2 * k + 1) - 1; });
    EXPECT_EQ(q2026_01::total_shortfall(stock, thresholds), 61L);
}

TEST(Event2026Quest01, TotalShortfallLongValuesDontOverflow) {
    const std::vector<long> stock{3'000'000'000L, 3'000'000'000L};
    const std::vector<long> thresholds{3'000'000'005L};
    EXPECT_EQ(q2026_01::total_shortfall(stock, thresholds), 10L);
}

TEST(Event2026Quest01, TotalShortfallThrowsWhenValueExceedsAllThresholds) {
    const std::vector<int> stock{3, 100};
    const std::vector<int> thresholds{1, 4, 9};
    EXPECT_THROW(q2026_01::total_shortfall(stock, thresholds), std::out_of_range);
}

// --- Examples from the puzzle text -----------------------------------------
// tools/fetch.py fills in the example and its answer while the raw string
// is still empty. Pasting by hand? Keep example text flush left.

TEST(Event2026Quest01, Part1Example) {
    checkExample(1, R"EX(
3,11,12,16,24,25
)EX", "11");
}

TEST(Event2026Quest01, Part2Example) {
    checkExample(2, R"EX(
3,11,12,16,24,25
)EX", "9");
}

TEST(Event2026Quest01, Part3Example) {
    checkExample(3, R"EX(
3,11,12,16,24,25
)EX", "61");
}

// --- Your accepted answers: a regression check for refactoring --------------
// Parts without notes on disk (e.g. a friend's clone) are skipped.

TEST(Event2026Quest01, MyAnswers) {
    const std::array<std::string, 3> accepted{"1426", "313", "626912"};  // fill in as you solve
    int checked = 0;
    for (int part = 1; part <= 3; ++part) {
        const auto input = vmc::notes(kEvent, kQuest, part);
        const auto& want = accepted[static_cast<std::size_t>(part - 1)];
        if (!input || want.empty()) continue;
        EXPECT_EQ(vmc::solve(kEvent, kQuest, part, *input), vmc::Answer(want)) << "part " << part;
        ++checked;
    }
    if (checked == 0) GTEST_SKIP() << "no accepted answers recorded (or no notes on disk)";
}

}  // namespace
