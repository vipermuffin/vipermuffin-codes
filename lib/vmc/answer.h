#pragma once
// An answer is just text: Everybody Codes answers are sometimes numbers,
// sometimes strings. Return whichever is natural; it converts automatically.

#include <concepts>
#include <ostream>
#include <string>
#include <utility>

namespace vmc {

struct Answer {
    std::string value;

    Answer() = default;
    Answer(std::string s) : value(std::move(s)) {}
    Answer(const char* s) : value(s) {}
    template <std::integral T>
    Answer(T v) : value(std::to_string(v)) {}

    bool empty() const { return value.empty(); }

    friend bool operator==(const Answer&, const Answer&) = default;
    friend std::ostream& operator<<(std::ostream& os, const Answer& a) { return os << a.value; }
};

}  // namespace vmc
