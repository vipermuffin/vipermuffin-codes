#include <array>
#include <string>

#include <gtest/gtest.h>

#include "q2025_03.h"

namespace {

constexpr const char* kEvent = "2025";
constexpr int kQuest = 3;

// Skips until the expected answer is filled in.
void checkExample(int part, std::string_view example, std::string expected) {
    if (expected.empty()) GTEST_SKIP() << "expected answer for part " << part << " not filled in yet";
    EXPECT_EQ(vmc::solve(kEvent, kQuest, part, example), vmc::Answer(expected));
}

// --- Unit tests for the pieces declared in q2025_03.h ------------------------
// TEST(Event2025Quest03, ParseExample) {
//     const auto x = q2025_03::parse("...");
//     EXPECT_EQ(x.size(), 3u);
// }

// --- Examples from the puzzle text -----------------------------------------
// tools/fetch.py fills in the example and its answer while the raw string
// is still empty. Pasting by hand? Keep example text flush left.

TEST(Event2025Quest03, Part1Example) {
    checkExample(1, R"EX(
10,5,1,10,3,8,5,2,2
)EX", "29");
}

TEST(Event2025Quest03, Part2Example) {
    checkExample(2, R"EX(
4,51,13,64,57,51,82,57,16,88,89,48,32,49,49,2,84,65,49,43,9,13,2,3,75,72,63,48,61,14,40,77
)EX", "781");
}

TEST(Event2025Quest03, Part3Example) {
    checkExample(3, R"EX(
4,51,13,64,57,51,82,57,16,88,89,48,32,49,49,2,84,65,49,43,9,13,2,3,75,72,63,48,61,14,40,77
)EX", "3");
}

// --- Your accepted answers: a regression check for refactoring --------------
// Parts without notes on disk (e.g. a friend's clone) are skipped.

TEST(Event2025Quest03, MyAnswers) {
    const std::array<std::string, 3> accepted{"2659", "297", "4084"};  // fill in as you solve
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
