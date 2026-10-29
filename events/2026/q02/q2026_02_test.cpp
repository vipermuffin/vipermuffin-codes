#include <array>
#include <string>

#include <gtest/gtest.h>

#include "q2026_02.h"

namespace {

constexpr const char* kEvent = "2026";
constexpr int kQuest = 2;

// Skips until the expected answer is filled in.
void checkExample(int part, std::string_view example, std::string expected) {
    if (expected.empty()) GTEST_SKIP() << "expected answer for part " << part << " not filled in yet";
    EXPECT_EQ(vmc::solve(kEvent, kQuest, part, example), vmc::Answer(expected));
}

// --- Unit tests for the pieces declared in q2026_02.h ------------------------
// TEST(Event2026Quest02, ParseExample) {
//     const auto x = q2026_02::parse("...");
//     EXPECT_EQ(x.size(), 3u);
// }
TEST(Event2026Quest02, GetChecksum) {
    const auto x = q2026_02::getChecksum("4<3 2<5 3>1 1>2 1<5 4<1 3>2 4<2 3>5 4<5");
    EXPECT_EQ(x, 35124);
}

TEST(Event2026Quest02, GetChecksumPt2) {
    const auto x = q2026_02::getChecksum("2<5 3>1 1>2 1<5 4<1 3>2 4<2 3>5 4<5");
    EXPECT_EQ(x, 35124);
}

TEST(Event2026Quest02, GetChecksumPt3) {
    const auto x = q2026_02::getChecksum("3>1 1>2 1<5 4<2 3>5");
    EXPECT_EQ(x, 35124);
}
// --- Examples from the puzzle text -----------------------------------------
// tools/fetch.py fills in the example and its answer while the raw string
// is still empty. Pasting by hand? Keep example text flush left.

TEST(Event2026Quest02, Part1Example) {
    checkExample(1, R"EX(
4<3 2<5 3>1 1>2 1<5 4<1 3>2 4<2 3>5 4<5
4>1 2>1 3>2 1<5 4>5 2<5 3>1 4>3 2<4 5<3
5>1 5>2 4>1 2<4 3>2 3<4 1>2 3>5 1<3 4>5
)EX", "122157");
}

TEST(Event2026Quest02, Part2Example) {
    checkExample(2, R"EX(
2<5 3>1 1>2 1<5 4<1 3>2 4<2 3>5 4<5
2>1 3>2 1<5 4>5 2<5 3>1 4>3 2<4 5<3
5>1 5>2 4>1 3>2 3<4 1>2 3>5 1<3 4>5
)EX", "122157");
}

TEST(Event2026Quest02, Part3Example) {
    checkExample(3, R"EX(
3>1 1>2 1<5 4<2 3>5
2>1 2<5 3>1 4>3 2<4 5<3
5>1 3<4 1>2 3>5
)EX", "122157");
}

// --- Your accepted answers: a regression check for refactoring --------------
// Parts without notes on disk (e.g. a friend's clone) are skipped.

TEST(Event2026Quest02, MyAnswers) {
    const std::array<std::string, 3> accepted{"368925", "61983383283", "595289224683"};  // fill in as you solve
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
