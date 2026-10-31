#pragma once
// Small text helpers. All return string_views into the caller's input,
// so keep the input string alive while you use them.

#include <cctype>
#include <cstddef>
#include <charconv>
#include <optional>
#include <string_view>
#include <type_traits>
#include <vector>

namespace vmc {

inline std::string_view trim(std::string_view s) {
    constexpr std::string_view ws = " \t\r\n";
    const auto b = s.find_first_not_of(ws);
    if (b == std::string_view::npos) return {};
    const auto e = s.find_last_not_of(ws);
    return s.substr(b, e - b + 1);
}

// Normalise puzzle input: drop leading empty lines (handy for raw-string
// examples in tests) and trailing newlines. Leading spaces on the first real
// line are kept, since grids can start with them.
inline std::string_view clean(std::string_view s) {
    while (!s.empty()) {
        const auto nl = s.find('\n');
        if (nl == std::string_view::npos) break;
        const auto first = s.substr(0, nl);
        if (!first.empty() && first != "\r") break;
        s.remove_prefix(nl + 1);
    }
    while (!s.empty() && (s.back() == '\n' || s.back() == '\r')) s.remove_suffix(1);
    return s;
}

inline std::vector<std::string_view> split(std::string_view s, std::string_view delim) {
    std::vector<std::string_view> out;
    if (delim.empty()) return {s};
    std::size_t pos = 0;
    for (;;) {
        const auto next = s.find(delim, pos);
        if (next == std::string_view::npos) {
            out.push_back(s.substr(pos));
            return out;
        }
        out.push_back(s.substr(pos, next - pos));
        pos = next + delim.size();
    }
}

inline std::vector<std::string_view> lines(std::string_view s) {
    auto v = split(s, "\n");
    for (auto& l : v)
        if (!l.empty() && l.back() == '\r') l.remove_suffix(1);
    return v;
}

// Sections separated by blank lines.
inline std::vector<std::string_view> blocks(std::string_view s) {
    std::vector<std::string_view> out;
    for (auto b : split(s, "\n\n"))
        if (auto t = trim(b); !t.empty()) out.push_back(b);
    return out;
}

// Every integer in the text, in order. A '-' counts as a minus sign only when
// it isn't glued to a preceding letter/digit, so "1-5" gives {1, 5} and
// "x=-3" gives {-3}.
template <class T = long long>
std::vector<T> ints(std::string_view s) {
    std::vector<T> out;
    const auto digit = [](char c) { return std::isdigit(static_cast<unsigned char>(c)) != 0; };
    for (std::size_t i = 0; i < s.size();) {
        const bool minus = s[i] == '-' && i + 1 < s.size() && digit(s[i + 1]) &&
                           (i == 0 || !std::isalnum(static_cast<unsigned char>(s[i - 1])));
        if (digit(s[i]) || minus) {
            T v{};
            const auto [p, err] = std::from_chars(s.data() + i, s.data() + s.size(), v);
            if (p == s.data() + i) { ++i; continue; }
            if (err == std::errc{}) out.push_back(v);
            i = static_cast<std::size_t>(p - s.data());
        } else {
            ++i;
        }
    }
    return out;
}

template <class T = long long>
T toInt(std::string_view s) {
    s = trim(s);
    T v{};
    std::from_chars(s.data(), s.data() + s.size(), v);
    return v;
}

// Consumes "key=<integer>" from the front of sv, plus one trailing space if present.
template <typename T = int>
    requires std::is_integral_v<T>
std::optional<T> parse_field(std::string_view& sv, std::string_view key) {
    if (!sv.starts_with(key)) return std::nullopt;
    sv.remove_prefix(key.size());
    if (sv.empty() || sv.front() != '=') return std::nullopt;
    sv.remove_prefix(1);

    T value{};
    auto [ptr, ec] = std::from_chars(sv.data(), sv.data() + sv.size(), value);
    if (ec != std::errc{}) return std::nullopt;
    sv.remove_prefix(static_cast<std::size_t>(ptr - sv.data()));

    if (!sv.empty() && sv.front() == ' ') sv.remove_prefix(1);
    return value;
}

}  // namespace vmc
