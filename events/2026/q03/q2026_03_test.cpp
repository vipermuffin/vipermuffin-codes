#include <array>
#include <string>

#include <gtest/gtest.h>

#include "q2026_03.h"

namespace {

constexpr const char* kEvent = "2026";
constexpr int kQuest = 3;

// Skips until the expected answer is filled in.
void checkExample(int part, std::string_view example, std::string expected) {
    if (expected.empty()) GTEST_SKIP() << "expected answer for part " << part << " not filled in yet";
    EXPECT_EQ(vmc::solve(kEvent, kQuest, part, example), vmc::Answer(expected));
}

// --- Unit tests for the pieces declared in q2026_03.h ------------------------
// TEST(Event2026Quest03, ParseExample) {
//     const auto x = q2026_03::parse("...");
//     EXPECT_EQ(x.size(), 3u);
// }
TEST(Event2026Quest03, ConvertVoteToDecimal) {
    const auto x = q2026_03::convertVoteToDecimalNotation("3210","7654");
    EXPECT_EQ(x, 222);
}

TEST(Event2026Quest03, VerifyIso2_1) {
    EXPECT_FALSE(q2026_03::isValidIso2("3 23"));
    EXPECT_FALSE(q2026_03::isValidIso2("61 149"));
    EXPECT_TRUE(q2026_03::isValidIso2("978 99"));
    EXPECT_TRUE(q2026_03::isValidIso2("5887 5923"));
    EXPECT_TRUE(q2026_03::isValidIso2("76988 123749"));
    EXPECT_TRUE(q2026_03::isValidIso2("435657 929977"));
}


// --- Examples from the puzzle text -----------------------------------------
// tools/fetch.py fills in the example and its answer while the raw string
// is still empty. Pasting by hand? Keep example text flush left.

TEST(Event2026Quest03, Part1Example) {
    checkExample(1, R"EX(
ISO-3210 7654
ISO-8742 2468
ISO-1111 1111
)EX", "1478");
}

TEST(Event2026Quest03, Part2Example) {
    checkExample(2, R"EX(
ISO-3210 7654
ISO-8742 2468
ISO-1 1111
ISO-13 777
ISO-34 140856
ISO-789 2465112
ISO-2341 945
)EX", "1161463");
}

TEST(Event2026Quest03, Part3Example) {
    checkExample(3, R"EX(
ISO2-3 23
ISO2-61 149
ISO2-978 99
ISO2-5887 5923
ISO2-76988 123749
ISO2-435657 929977
ISO2-3949 870737
)EX", "50987");
}

// --- Your accepted answers: a regression check for refactoring --------------
// Parts without notes on disk (e.g. a friend's clone) are skipped.

TEST(Event2026Quest03, MyAnswers) {
    const std::array<std::string, 3> accepted{"17458", "24142018852", "423946"};  // fill in as you solve
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
