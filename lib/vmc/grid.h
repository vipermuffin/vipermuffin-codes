#pragma once
// 2D grid helpers. Coordinates are (row, col): row grows downward, col grows
// to the right, matching how the puzzle text is laid out on screen.

#include <algorithm>
#include <array>
#include <compare>
#include <cstdlib>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "vmc/parse.h"

namespace vmc {

struct Point {
    int r = 0;
    int c = 0;

    constexpr Point operator+(Point o) const { return {r + o.r, c + o.c}; }
    constexpr Point operator-(Point o) const { return {r - o.r, c - o.c}; }
    constexpr Point operator*(int k) const { return {r * k, c * k}; }
    constexpr Point& operator+=(Point o) { r += o.r; c += o.c; return *this; }
    constexpr Point& operator-=(Point o) { r -= o.r; c -= o.c; return *this; }

    constexpr Point turnRight() const { return {c, -r}; }  // as a direction
    constexpr Point turnLeft() const { return {-c, r}; }

    friend constexpr auto operator<=>(const Point&, const Point&) = default;
};

constexpr int manhattan(Point a, Point b) { return std::abs(a.r - b.r) + std::abs(a.c - b.c); }

inline constexpr Point UP{-1, 0}, RIGHT{0, 1}, DOWN{1, 0}, LEFT{0, -1};
inline constexpr std::array<Point, 4> DIRS4{UP, RIGHT, DOWN, LEFT};
inline constexpr std::array<Point, 8> DIRS8{{{-1, -1}, {-1, 0}, {-1, 1}, {0, 1},
                                             {1, 1},   {1, 0},  {1, -1}, {0, -1}}};

template <class T>
class Grid {
public:
    Grid() = default;
    Grid(int rows, int cols, T fill = T{})
        : rows_(rows), cols_(cols), cells_(static_cast<std::size_t>(rows) * cols, fill) {}

    int rows() const { return rows_; }
    int cols() const { return cols_; }
    bool contains(Point p) const { return p.r >= 0 && p.r < rows_ && p.c >= 0 && p.c < cols_; }

    // Grid<bool> works too, thanks to vector<bool>'s proxy reference type.
    typename std::vector<T>::reference operator[](Point p) { return cells_[index(p)]; }
    typename std::vector<T>::const_reference operator[](Point p) const { return cells_[index(p)]; }

    // Value at p, or `outside` when p is off the grid.
    T get(Point p, T outside) const { return contains(p) ? cells_[index(p)] : outside; }

    std::vector<Point> points() const {
        std::vector<Point> out;
        out.reserve(cells_.size());
        for (int r = 0; r < rows_; ++r)
            for (int c = 0; c < cols_; ++c) out.push_back({r, c});
        return out;
    }

    std::vector<Point> neighbours4(Point p) const {
        std::vector<Point> out;
        for (auto d : DIRS4)
            if (contains(p + d)) out.push_back(p + d);
        return out;
    }

    std::vector<Point> neighbours8(Point p) const {
        std::vector<Point> out;
        for (auto d : DIRS8)
            if (contains(p + d)) out.push_back(p + d);
        return out;
    }

    std::optional<Point> find(const T& v) const {
        for (auto p : points())
            if ((*this)[p] == v) return p;
        return std::nullopt;
    }

    std::vector<Point> findAll(const T& v) const {
        std::vector<Point> out;
        for (auto p : points())
            if ((*this)[p] == v) out.push_back(p);
        return out;
    }

private:
    std::size_t index(Point p) const { return static_cast<std::size_t>(p.r) * cols_ + p.c; }

    int rows_ = 0;
    int cols_ = 0;
    std::vector<T> cells_;
};

using CharGrid = Grid<char>;

// Ragged lines are padded with `pad` to the widest line.
inline CharGrid parseGrid(std::string_view text, char pad = ' ') {
    const auto ls = lines(text);
    std::size_t width = 0;
    for (auto l : ls) width = std::max(width, l.size());
    CharGrid g(static_cast<int>(ls.size()), static_cast<int>(width), pad);
    for (int r = 0; r < g.rows(); ++r)
        for (int c = 0; c < static_cast<int>(ls[r].size()); ++c) g[{r, c}] = ls[r][c];
    return g;
}

inline std::string toString(const CharGrid& g) {
    std::string s;
    for (int r = 0; r < g.rows(); ++r) {
        for (int c = 0; c < g.cols(); ++c) s += g[{r, c}];
        s += '\n';
    }
    return s;
}

}  // namespace vmc

template <>
struct std::hash<vmc::Point> {
    std::size_t operator()(vmc::Point p) const noexcept {
        return std::hash<long long>{}((static_cast<long long>(p.r) << 32) ^
                                      static_cast<unsigned int>(p.c));
    }
};
