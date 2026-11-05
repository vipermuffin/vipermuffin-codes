#include <array>
#include <string>

#include <gtest/gtest.h>

#include "q2026_08.h"

namespace {

constexpr const char* kEvent = "2026";
constexpr int kQuest = 8;

// Skips until the expected answer is filled in.
void checkExample(int part, std::string_view example, std::string expected) {
    if (expected.empty()) GTEST_SKIP() << "expected answer for part " << part << " not filled in yet";
    EXPECT_EQ(vmc::solve(kEvent, kQuest, part, example), vmc::Answer(expected));
}

// --- Unit tests for the pieces declared in q2026_08.h ------------------------
// TEST(Event2026Quest08, ParseExample) {
//     const auto x = q2026_08::parse("...");
//     EXPECT_EQ(x.size(), 3u);
// }
TEST(Event2026Quest08, calcEnergy) {
    const auto x = q2026_08::calcLineEnergy({4,9,7,9,1});
    EXPECT_EQ(x, 3);
}
// --- Examples from the puzzle text -----------------------------------------
// tools/fetch.py fills in the example and its answer while the raw string
// is still empty. Pasting by hand? Keep example text flush left.

TEST(Event2026Quest08, Part1Example) {
    checkExample(1, R"EX(
....1.9.6.1.7....
...9.2.3.3.8.1...
..7.8.3.5.1.4.3..
.4.6.7.4.1.1.8.9.
4.5.3.6.5.8.9.4.9
.9.5.1.5.6.2.8.9.
..7.7.6.4.6.2.4..
...9.3.8.3.8.4...
....1.6.6.2.8....
)EX", "32");
}

TEST(Event2026Quest08, Part2Example) {
    checkExample(2, R"EX(
....1.9.6.1.7....
...9.2.3.3.8.1...
..7.8.3.5.1.4.3..
.4.6.7.4.1.1.8.9.
4.5.3.6.5.8.9.4.9
.9.5.1.5.6.2.8.9.
..7.7.6.4.6.2.4..
...9.3.8.3.8.4...
....1.6.6.2.8....
)EX", "187");
}

TEST(Event2026Quest08, Part3Example) {
    checkExample(3, R"EX(
....1.9.6.1.7....
...9.2.3.3.8.1...
..7.8.3.5.1.4.3..
.4.6.7.4.1.1.8.9.
4.5.3.6.5.8.9.4.9
.9.5.1.5.6.2.8.9.
..7.7.6.4.6.2.4..
...9.3.8.3.8.4...
....1.6.6.2.8....
)EX", "899889");
}

// --- Your accepted answers: a regression check for refactoring --------------
// Parts without notes on disk (e.g. a friend's clone) are skipped.

TEST(Event2026Quest08, MyAnswers) {
    const std::array<std::string, 3> accepted{"182", "1031", ""};  // fill in as you solve
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
