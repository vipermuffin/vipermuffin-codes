#pragma once
// Each quest registers its three part functions with VMC_QUEST(...). The
// runner and the tests look quests up here, so adding a quest never means
// editing a list somewhere else.

#include <array>
#include <filesystem>
#include <fstream>
#include <map>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include "vmc/answer.h"
#include "vmc/parse.h"

namespace vmc {

using Part = Answer (*)(std::string_view);

struct Quest {
    std::string event;
    int number = 0;
    std::array<Part, 3> parts{};
    std::filesystem::path dir;  // folder holding the quest source and its notes

    std::filesystem::path notesPath(int part) const {
        return dir / ("notes_p" + std::to_string(part) + ".txt");
    }
};

inline std::map<std::pair<std::string, int>, Quest>& registry() {
    static std::map<std::pair<std::string, int>, Quest> r;
    return r;
}

inline const Quest* findQuest(const std::string& event, int quest) {
    const auto it = registry().find({event, quest});
    return it == registry().end() ? nullptr : &it->second;
}

struct Registrar {
    Registrar(const char* event, int quest, Part p1, Part p2, Part p3, const char* file) {
        registry()[{event, quest}] =
            Quest{event, quest, {p1, p2, p3}, std::filesystem::path(file).parent_path()};
    }
};

inline std::optional<std::string> readFile(const std::filesystem::path& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return std::nullopt;
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

// Your notes (puzzle input) for a part, if they've been fetched.
inline std::optional<std::string> notes(const std::string& event, int quest, int part) {
    const auto* q = findQuest(event, quest);
    return q ? readFile(q->notesPath(part)) : std::nullopt;
}

// Run one part on some input. Used by the runner and by tests.
inline Answer solve(const std::string& event, int quest, int part, std::string_view input) {
    const auto* q = findQuest(event, quest);
    if (!q) throw std::runtime_error("quest not registered: " + event + " " + std::to_string(quest));
    if (part < 1 || part > 3) throw std::out_of_range("part must be 1, 2 or 3");
    const Part fn = q->parts[static_cast<std::size_t>(part - 1)];
    if (!fn) return {};
    return fn(clean(input));
}

}  // namespace vmc

#define VMC_CONCAT_IMPL(a, b) a##b
#define VMC_CONCAT(a, b) VMC_CONCAT_IMPL(a, b)

// VMC_QUEST("2026", 3, part1, part2, part3)
#define VMC_QUEST(event, quest, p1, p2, p3)                                               \
    namespace {                                                                           \
    const ::vmc::Registrar VMC_CONCAT(vmc_registrar_, __LINE__){event, quest, p1, p2, p3, \
                                                                __FILE__};                \
    }
