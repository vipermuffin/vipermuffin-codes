#pragma once
// Everybody Codes 2025 - Quest 2
// Declare the pieces you want to unit test (input type, parse(), one step, ...)
// and define them in q2025_02.cpp inside the same namespace.
#include "vmc/vmc.h"
#include <format>
#include <string>

namespace q2025_02 {
    template <typename T>
    struct EC_Complex {
    T x;
    T y;

    EC_Complex() = delete;                              // no default construction
    constexpr EC_Complex(T x, T y) : x(x), y(y) {}  // member-init list

    constexpr EC_Complex operator+(const EC_Complex& o) const { return {x + o.x, y + o.y}; }
    constexpr EC_Complex operator-(const EC_Complex& o) const { return {x - o.x, y - o.y}; }
    constexpr EC_Complex operator*(const EC_Complex& o) const { return {x * o.x - y * o.y, x * o.y + y * o.x}; }
    constexpr EC_Complex operator/(const EC_Complex& o) const { return {x / o.x, y / o.y}; }
    std::string str() const { return std::format("[{},{}]", x, y); }
};

    // One engraving cycle: square, divide, then add.
    template <typename T>
    constexpr EC_Complex<T> performCycle(const EC_Complex<T>& r, const EC_Complex<T>& divisor, const EC_Complex<T>& add) {
        return (r * r) / divisor + add;
    }

    // True if the point stays within +/-1,000,000 for all 100 cycles.
    bool shouldEngrave(const EC_Complex<long>& p);

    // Counts engraved points on the 1001x1001 grid starting at (x, y), stepping by `step`.
    int countEngravedPointsWithResolution(int x, int y, int step);

}  // namespace q2025_02
