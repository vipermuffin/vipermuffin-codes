#include <array>
#include <string>

#include <gtest/gtest.h>

#include "q2026_16.h"

namespace {

constexpr const char* kEvent = "2026";
constexpr int kQuest = 16;

// Skips until the expected answer is filled in.
void checkExample(int part, std::string_view example, std::string expected) {
    if (expected.empty()) GTEST_SKIP() << "expected answer for part " << part << " not filled in yet";
    EXPECT_EQ(vmc::solve(kEvent, kQuest, part, example), vmc::Answer(expected));
}

// --- Unit tests for the pieces declared in q2026_16.h ------------------------
// TEST(Event2026Quest16, ParseExample) {
//     const auto x = q2026_16::parse("...");
//     EXPECT_EQ(x.size(), 3u);
// }

// --- Examples from the puzzle text -----------------------------------------
// tools/fetch.py fills in the example and its answer while the raw string
// is still empty. Pasting by hand? Keep example text flush left.

TEST(Event2026Quest16, Part1aExample) {
    checkExample(1, R"EX(
RRGGBBGRRGGBBG
)EX", "3");
}

TEST(Event2026Quest16, Part1bExample) {
    checkExample(1, R"EX(
BBRGGRRGBBRGGBRGBBRRBRRRBGGRRRBGBGGBBRGGRRGBBRGGBRGBBRRBRRRBGGRRRBGBGG
)EX", "26");
}

TEST(Event2026Quest16, Part2Example) {
    checkExample(2, R"EX(
RRYGYBYG
)EX", "1053");
}

TEST(Event2026Quest16, Part3Example) {
    checkExample(3, R"EX(
RRYGGYBBYG
)EX", "135154");
}

// --- Your accepted answers: a regression check for refactoring --------------
// Parts without notes on disk (e.g. a friend's clone) are skipped.

TEST(Event2026Quest16, MyAnswers) {
    const std::array<std::string, 3> accepted{"57", "24666283", "24760803004036"};  // fill in as you solve
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
