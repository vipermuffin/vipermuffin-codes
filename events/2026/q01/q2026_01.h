#pragma once
// Everybody Codes 2026 - Quest 1
// Declare the pieces you want to unit test (input type, parse(), one step, ...)
// and define them in q2026_01.cpp inside the same namespace.

#include <concepts>
#include <format>
#include <ranges>
#include <stdexcept>

#include "vmc/vmc.h"

namespace q2026_01 {

// For each value in `stock`, find the smallest threshold >= that value and add
// up the differences (threshold - value).
//
// - `thresholds` must be sorted ascending. It can be a container or a lazy view
//   (e.g. iota | transform); random access lets you binary search it.
// - Throws std::out_of_range if a value is larger than every threshold.
//
// It's a template, so the body has to live here in the header, not in the .cpp.
template <std::ranges::input_range Stock, std::ranges::random_access_range Thresholds>
    requires std::integral<std::ranges::range_value_t<Stock>>
std::ranges::range_value_t<Stock> total_shortfall([[maybe_unused]] const Stock& stock,
                                                  [[maybe_unused]] const Thresholds& thresholds) {
    std::ranges::range_value_t<Stock> needed{0};
    for(const auto x : stock) {
        auto it = std::ranges::lower_bound(thresholds,x);
        if(it == std::ranges::end(thresholds)) {
            throw std::out_of_range(std::format("Value {} exceeds all thresholds",x));
        }
        needed += *it - x;
    }
    return needed;
}

}  // namespace q2026_01
