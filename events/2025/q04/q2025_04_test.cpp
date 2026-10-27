#include <array>
#include <string>

#include <gtest/gtest.h>

#include "q2025_04.h"

namespace {

constexpr const char* kEvent = "2025";
constexpr int kQuest = 4;

// Skips until the expected answer is filled in.
void checkExample(int part, std::string_view example, std::string expected) {
    if (expected.empty()) GTEST_SKIP() << "expected answer for part " << part << " not filled in yet";
    EXPECT_EQ(vmc::solve(kEvent, kQuest, part, example), vmc::Answer(expected));
}

// --- Unit tests for the pieces declared in q2025_04.h ------------------------
TEST(Event2025Quest04, GearRatioTestWithInts) {
    std::vector<int> x{128,64,32,16,8};
    EXPECT_EQ(q2025_04::calculateGearRatio<int>(x), 16);
}

TEST(Event2025Quest04, GearRatioTestWithDoubles) {
    std::vector<double> x{128,64,32,16,8};
    EXPECT_DOUBLE_EQ(q2025_04::calculateGearRatio<double>(x), 16.0);
}

TEST(Event2025Quest04, GearRatioTestWithDoublesAndNotEven) {
    std::vector<double> x{102,75,50,35,13};
    EXPECT_NEAR(q2025_04::calculateGearRatio<double>(x), 7.84615385, .00000001);
}

TEST(Event2025Quest04, PairedGearWithRatioEvenRatio) {
    std::vector<std::pair<double,double>> x{{5,5},{5,10},{10,20},{5,5}};
    EXPECT_DOUBLE_EQ(q2025_04::calculatePairedGearRatio<double>(x), 4.0);
}

// --- Examples from the puzzle text -----------------------------------------
// tools/fetch.py fills in the example and its answer while the raw string
// is still empty. Pasting by hand? Keep example text flush left.

TEST(Event2025Quest04, Part1Example) {
    checkExample(1, R"EX(
128
64
32
16
8
)EX", "32400");
}

TEST(Event2025Quest04, Part2Example) {
    checkExample(2, R"EX(
102
75
50
35
13
)EX", "1274509803922");
}

TEST(Event2025Quest04, Part3Example) {
    checkExample(3, R"EX(
5
7|21
18|36
27|27
10|50
10|50
11
)EX", "6818");
}

// --- Your accepted answers: a regression check for refactoring --------------
// Parts without notes on disk (e.g. a friend's clone) are skipped.

TEST(Event2025Quest04, MyAnswers) {
    const std::array<std::string, 3> accepted{"24397", "1865671641792", "171110253277"};  // fill in as you solve
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
