#pragma once
// Everybody Codes 2026 - Quest 4
// Declare the pieces you want to unit test (input type, parse(), one step, ...)
// and define them in q2026_04.cpp inside the same namespace.

#include "vmc/vmc.h"
#include <string_view>


namespace q2026_04 {
    struct Result {
        Result(int id, int quest, int part, int time):
        id_(id),quest_(quest), part_(part),time_(time) {}
        Result(std::string_view s);
        std::strong_ordering operator<=>(const Result rhs) const {
            if(auto c = quest_ <=> rhs.quest_; c != 0) return c;
            if(auto c = part_ <=> rhs.part_; c != 0) return c;
            return time_ <=> rhs.time_;
        }
        bool operator==(const Result& rhs) const {
            return quest_ == rhs.quest_ && part_ == rhs.part_ && time_ == rhs.time_;
        };
        int id_;
        int quest_;
        int part_;
        int time_;
    };

}  // namespace q2026_04
