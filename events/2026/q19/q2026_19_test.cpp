#include <array>
#include <string>

#include <gtest/gtest.h>

#include "q2026_19.h"

namespace {

constexpr const char* kEvent = "2026";
constexpr int kQuest = 19;

// Skips until the expected answer is filled in.
void checkExample(int part, std::string_view example, std::string expected) {
    if (expected.empty()) GTEST_SKIP() << "expected answer for part " << part << " not filled in yet";
    EXPECT_EQ(vmc::solve(kEvent, kQuest, part, example), vmc::Answer(expected));
}

// --- Unit tests for the pieces declared in q2026_19.h ------------------------
// TEST(Event2026Quest19, ParseExample) {
//     const auto x = q2026_19::parse("...");
//     EXPECT_EQ(x.size(), 3u);
// }

// --- Examples from the puzzle text -----------------------------------------
// tools/fetch.py fills in the example and its answer while the raw string
// is still empty. Pasting by hand? Keep example text flush left.

TEST(Event2026Quest19, Part1Example) {
    checkExample(1, R"EX(
CCCCC
MMMMM
YYYYY
KKKKK
CCMYY

CMYKC
CMYKC
CMYKC
CMYKC
CMYKC
)EX", "144");
}

TEST(Event2026Quest19, Part2Example) {
    checkExample(2, R"EX(
CCCCC
MMMMM
YYYYY
KKKKK
CCMYY

14231
14231
14231
14231
14231
)EX", "180");
}

TEST(Event2026Quest19, Part3aExample) {
    checkExample(3, R"EX(
CCMM
YYKK
)EX", "18");
}

TEST(Event2026Quest19, Part3bExample) {
    checkExample(3, R"EX(
CCCMMM
CCMMYY
CCCYKK
CMMYYK
)EX", "343");
}

TEST(Event2026Quest19, Part3cExample) {
    checkExample(3, R"EX(
CCCCYMMM
CCMMMYYY
CCCYYYMM
MMCCCYYY
YYYMMMCC
CCYYMMKK
MMYYCCKK
YYYCCMMM
)EX", "8000");
}

// --- Your accepted answers: a regression check for refactoring --------------
// Parts without notes on disk (e.g. a friend's clone) are skipped.

TEST(Event2026Quest19, MyAnswers) {
    const std::array<std::string, 3> accepted{"2945952", "278546088132", "2050641885600"};  // fill in as you solve
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
