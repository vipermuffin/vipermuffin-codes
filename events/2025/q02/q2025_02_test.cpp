#include <array>
#include <string>

#include <gtest/gtest.h>

#include "q2025_02.h"

namespace {

constexpr const char* kEvent = "2025";
constexpr int kQuest = 2;

// Skips until the expected answer is filled in.
void checkExample(int part, std::string_view example, std::string expected) {
    if (expected.empty()) GTEST_SKIP() << "expected answer for part " << part << " not filled in yet";
    EXPECT_EQ(vmc::solve(kEvent, kQuest, part, example), vmc::Answer(expected));
}

// --- Unit tests for the pieces declared in q2025_02.h ------------------------
TEST(Event2025Quest02, PerformCycleChain) {
    using C = q2025_02::EC_Complex<int>;
    const C divisor(10, 10);
    const C add(25, 9);

    C r(0, 0);
    r = q2025_02::performCycle(r, divisor, add);
    EXPECT_EQ(r.str(), "[25,9]");
    r = q2025_02::performCycle(r, divisor, add);
    EXPECT_EQ(r.str(), "[79,54]");
    r = q2025_02::performCycle(r, divisor, add);
    EXPECT_EQ(r.str(), "[357,862]");
}

TEST(Event2025Quest02, PerformCycleEachStepIndependently) {
    using C = q2025_02::EC_Complex<int>;
    const C divisor(10, 10);
    const C add(25, 9);
    EXPECT_EQ(q2025_02::performCycle(C(0, 0), divisor, add).str(), "[25,9]");
    EXPECT_EQ(q2025_02::performCycle(C(25, 9), divisor, add).str(), "[79,54]");
    EXPECT_EQ(q2025_02::performCycle(C(79, 54), divisor, add).str(), "[357,862]");
}

TEST(Event2025Quest02, ShouldEngraveValidPoints) {
    const std::array<std::array<long, 2>, 5> points{{
        {35630, -64880}, {35630, -64870}, {35640, -64860}, {36230, -64270}, {36250, -64270}}};
    for (const auto& p : points) {
        EXPECT_TRUE(q2025_02::shouldEngrave({p[0], p[1]})) << "P=[" << p[0] << "," << p[1] << "]";
    }
}

TEST(Event2025Quest02, ShouldNotEngraveInvalidPoints) {
    const std::array<std::array<long, 2>, 5> points{{
        {35460, -64910}, {35470, -64910}, {35480, -64910}, {35680, -64850}, {35630, -64830}}};
    for (const auto& p : points) {
        EXPECT_FALSE(q2025_02::shouldEngrave({p[0], p[1]})) << "P=[" << p[0] << "," << p[1] << "]";
    }
}

TEST(Event2025Quest02, CountEngravedPointsWithResolution) {
    EXPECT_EQ(q2025_02::countEngravedPointsWithResolution(35300, -64910, 10), 4076);
    EXPECT_EQ(q2025_02::countEngravedPointsWithResolution(35300, -64910, 1), 406954);
}

// --- Examples from the puzzle text -----------------------------------------
// tools/fetch.py fills in the example and its answer while the raw string
// is still empty. Pasting by hand? Keep example text flush left.

TEST(Event2025Quest02, Part1Example) {
    checkExample(1, R"EX(
A=[25,9]
)EX", "[357,862]");
}

TEST(Event2025Quest02, Part2Example) {
    checkExample(2, R"EX(
A=[35300,-64910]
)EX", "4076");
}

TEST(Event2025Quest02, Part3Example) {
    checkExample(3, R"EX(
A=[35300,-64910]
)EX", "406954");
}

// --- Your accepted answers: a regression check for refactoring --------------
// Parts without notes on disk (e.g. a friend's clone) are skipped.

TEST(Event2025Quest02, MyAnswers) {
    const std::array<std::string, 3> accepted{"[280965,993088]", "1182", "110989"};  // fill in as you solve
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
