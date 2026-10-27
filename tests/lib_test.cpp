// Tests for the shared helpers in lib/vmc. Add a test whenever you add a helper.

#include <unordered_set>

#include <gtest/gtest.h>

#include "vmc/vmc.h"

TEST(Parse, IntsHandlesNegativesAndRanges) {
    EXPECT_EQ(vmc::ints("x=-3, y=12"), (std::vector<long long>{-3, 12}));
    EXPECT_EQ(vmc::ints("1-5 a-2"), (std::vector<long long>{1, 5, 2}));
    EXPECT_EQ(vmc::ints<int>("no numbers"), std::vector<int>{});
}

TEST(Parse, LinesStripsCarriageReturns) {
    EXPECT_EQ(vmc::lines("ab\r\ncd"), (std::vector<std::string_view>{"ab", "cd"}));
}

TEST(Parse, CleanKeepsLeadingSpacesOfGrid) {
    EXPECT_EQ(vmc::clean("\n\n  #\n# \n\n"), "  #\n# ");
}

TEST(Parse, Blocks) {
    EXPECT_EQ(vmc::blocks("a\nb\n\nc"), (std::vector<std::string_view>{"a\nb", "c"}));
}

TEST(Grid, ParsePadsRaggedLines) {
    const auto g = vmc::parseGrid("ab\nc");
    EXPECT_EQ(g.rows(), 2);
    EXPECT_EQ(g.cols(), 2);
    EXPECT_EQ((g[{1, 1}]), ' ');
    EXPECT_EQ(g.find('c'), (vmc::Point{1, 0}));
    EXPECT_EQ(vmc::toString(g), "ab\nc \n");
}

TEST(Grid, PointTurnsAndHashes) {
    EXPECT_EQ(vmc::UP.turnRight(), vmc::RIGHT);
    EXPECT_EQ(vmc::RIGHT.turnRight(), vmc::DOWN);
    EXPECT_EQ(vmc::UP.turnLeft(), vmc::LEFT);
    std::unordered_set<vmc::Point> seen{{1, 2}, {1, 2}, {2, 1}};
    EXPECT_EQ(seen.size(), 2u);
}

TEST(Grid, BoolGridWorks) {
    vmc::Grid<bool> g(2, 3, false);
    g[{1, 2}] = true;
    EXPECT_TRUE((g[{1, 2}]));
    EXPECT_FALSE(g.get({5, 5}, false));
}
