#include <array>
#include <string>

#include <gtest/gtest.h>

#include "q2026_06.h"

namespace {

constexpr const char* kEvent = "2026";
constexpr int kQuest = 6;

// Skips until the expected answer is filled in.
void checkExample(int part, std::string_view example, std::string expected) {
    if (expected.empty()) GTEST_SKIP() << "expected answer for part " << part << " not filled in yet";
    EXPECT_EQ(vmc::solve(kEvent, kQuest, part, example), vmc::Answer(expected));
}

// --- Unit tests for the pieces declared in q2026_06.h ------------------------
// TEST(Event2026Quest06, ParseExample) {
//     const auto x = q2026_06::parse("...");
//     EXPECT_EQ(x.size(), 3u);
// }

// --- Examples from the puzzle text -----------------------------------------
// tools/fetch.py fills in the example and its answer while the raw string
// is still empty. Pasting by hand? Keep example text flush left.

TEST(Event2026Quest06, Part1Example) {
    checkExample(1, R"EX(
BRDS:FLWR,SLTT,YSTS
FLWR:SLTT,YSTS
SLTT:YSTS,MLKK,BTTR
NULL:BRDS,FLWR,SLTT
YSTS:BRDS,FLWR
MLKK:SPCS,NULL
HNYN:BRDS,FLWR,SLTT,YSTS
)EX", "7");
}

TEST(Event2026Quest06, Part2Example) {
    checkExample(2, R"EX(
BRDS:FLWR,SLTT,YSTS
FLWR:SLTT,YSTS
SLTT:YSTS,MLKK,BTTR
NULL:BRDS,FLWR,SLTT
YSTS:BRDS,FLWR
MLKK:SPCS,NULL
HNYN:BRDS,FLWR,SLTT,YSTS
)EX", "6");
}

TEST(Event2026Quest06, Part3Example) {
    checkExample(3, R"EX(
MEAL:BRDS,CAKE,DOUG
BRDS:DOUG,FLWR
CAKE:DOUG,FLWR,NULLv2,NULLv3
DOUG:FLWR,YSTS,SLTT,NULLv1
FLWR:BRDS,DOUG
YSTS:SPCS
SOUP:BRDS,CAKE,DOUG,FLWR
)EX", "3");
}

// --- Your accepted answers: a regression check for refactoring --------------
// Parts without notes on disk (e.g. a friend's clone) are skipped.

TEST(Event2026Quest06, MyAnswers) {
    const std::array<std::string, 3> accepted{"40", "827", "1667"};  // fill in as you solve
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
