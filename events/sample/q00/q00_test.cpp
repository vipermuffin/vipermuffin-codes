#include <gtest/gtest.h>

#include "vmc/vmc.h"

namespace {

constexpr const char* kEvent = "sample";
constexpr int kQuest = 0;

TEST(EventsampleQuest00, Part1Example) {
    EXPECT_EQ(vmc::solve(kEvent, kQuest, 1, R"(
3 -1 7
10
)"), 19);
}

TEST(EventsampleQuest00, Part2Example) {
    EXPECT_EQ(vmc::solve(kEvent, kQuest, 2, R"(
.###.
#####
.###.
)"), 3);
}

TEST(EventsampleQuest00, Part3Example) {
    EXPECT_EQ(vmc::solve(kEvent, kQuest, 3, R"(
S..#
.#.#
...E
)"), 5);
}

TEST(EventsampleQuest00, MyAnswers) {
    for (int part = 1; part <= 3; ++part) ASSERT_TRUE(vmc::notes(kEvent, kQuest, part)) << part;
    EXPECT_EQ(vmc::solve(kEvent, kQuest, 1, *vmc::notes(kEvent, kQuest, 1)), 61);
    EXPECT_EQ(vmc::solve(kEvent, kQuest, 2, *vmc::notes(kEvent, kQuest, 2)), 22);
    EXPECT_EQ(vmc::solve(kEvent, kQuest, 3, *vmc::notes(kEvent, kQuest, 3)), 21);
}

}  // namespace
