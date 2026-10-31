// Everybody Codes 2026 - Quest 4
// Approach write-up: NOTES.md in this folder.

#include "q2026_04.h"
#include <unordered_map>
#include <array>
#include <algorithm>
#include <compare>
#include <ranges>
namespace q2026_04 {

// Shared parsing and core logic. The three parts usually build on one idea
// and scale it up, so keep part1..3 thin and test the pieces from here.
Result::Result(std::string_view s) : id_{}, quest_{}, part_{}, time_{} {
    auto id    = vmc::parse_field(s, "id");
    auto quest = vmc::parse_field(s, "quest");
    auto part  = vmc::parse_field(s, "part");
    auto time  = vmc::parse_field(s, "time");
    if (!id || !quest || !part || !time)
        throw std::invalid_argument("malformed Result string");

    id_    = *id;
    quest_ = *quest;
    part_  = *part;
    time_  = *time;
}

struct Participant {
    Participant() : id(0), completed(0), time{0}{}
    Participant(int pid) : id(pid), completed(0), time{0}{}
    int totalTime() const {return time;}
    void addTime([[maybe_unused]]int p, int t) {time+=t;completed++;}
    // bool operator<(const Participant& rhs) const {return  totalTime() < rhs.totalTime(); }
    std::strong_ordering operator<=>(const Participant rhs) const {
        if(auto c = rhs.completed <=> completed; c != 0) return c;
        if(auto c = totalTime() <=> rhs.totalTime(); c != 0) return c;
        return id <=> rhs.id;
    }
    bool operator==(const Participant& rhs) const = default;
    int id;
    int completed;
    long time;
};

}  // namespace q2026_04

namespace {

using namespace q2026_04;

vmc::Answer part1([[maybe_unused]] std::string_view input) {
    auto lines = vmc::lines(input);
    std::vector<Result> results{};
    std::ranges::for_each(lines,[&](std::string_view sv){results.emplace_back(sv);});

    std::unordered_map<int,Participant> participants{};
    std::ranges::for_each(results,[&](const Result& r) {participants.try_emplace(r.id_,r.id_);participants[r.id_].addTime(r.part_, r.time_);});
    auto vals = participants | std::views::values;
    std::vector<Participant> pv{vals.begin(),vals.end()};
    // std::ranges::for_each(participants,[&](const std::pair<int,Participant>& p){pv.push_back(p.second);});
    // Sort using <=>
    std::ranges::sort(pv);
    
    int ttime = 0;
    int i=1;
    std::ranges::for_each(pv,[&](const Participant& p){ttime += p.totalTime()*i++;});

    return {ttime};
}

vmc::Answer part2(std::string_view input) {
    auto lines = vmc::lines(input);
    std::vector<Result> results{};
    std::ranges::for_each(lines,[&](std::string_view sv){results.emplace_back(sv);});

    std::unordered_map<int,Participant> participants{};
    std::ranges::for_each(results,[&](const Result& r) {if(participants.count(r.id_)==0) {participants[r.id_].id=r.id_;} participants[r.id_].addTime(r.part_, r.time_);});
    auto vals = participants | std::views::values;
    std::vector<Participant> pv{vals.begin(),vals.end()};
    // Sort using a projection
    std::ranges::sort(pv, {}, [](const Participant& p){ return std::tuple(-p.completed, p.time);});
    int ttime = 0;
    int i=1;
    
    std::ranges::for_each(pv,[&](const Participant& p){ttime += p.totalTime()*i++;});

    return {ttime};
}

vmc::Answer part3([[maybe_unused]] std::string_view input) {
    auto lines = vmc::lines(input);
    std::vector<Result> results{};
    int maxQuests{0};
    std::ranges::for_each(lines,[&](std::string_view sv){results.emplace_back(sv);if(results.back().quest_ > maxQuests) {maxQuests= results.back().quest_;}});
    std::ranges::for_each(results,[&](Result& r) {auto itr = results.begin(); while(itr != results.end()) {if(itr->id_ == r.id_ && itr->quest_ == r.quest_ && itr->part_ == 2 && r.part_ == 1) {itr->time_+= r.time_;}++itr;}});
    std::ranges::for_each(results,[&](Result& r) {auto itr = results.begin(); while(itr != results.end()) {if(itr->id_ == r.id_ && itr->quest_ == r.quest_ && itr->part_ == 3 && r.part_ == 2) {itr->time_+= r.time_;}++itr;}});
    std::ranges::sort(results);
    std::array<int,3> points {50,100,150};
    std::vector<std::array<int,3>> pointsAvailable(maxQuests,points);
    std::unordered_map<int, long> playerPoints{};
    Result prevResult = results.back();
    int tieCount = 0;
    std::ranges::for_each(results,[&](const Result& r) {if(prevResult == r) {++tieCount;}else{tieCount=0;} if(pointsAvailable[r.quest_-1][r.part_-1]+tieCount>=0) {playerPoints[r.id_] += pointsAvailable[r.quest_-1][r.part_-1]--+tieCount;} else {pointsAvailable[r.quest_-1][r.part_-1]=-222;} prevResult = r;});
    std::vector<long> pv{};
    std::ranges::for_each(playerPoints,[&](const std::pair<int,long> kvp){pv.push_back(kvp.second);});
    std::ranges::sort(pv, std::ranges::greater{});

    long ttime = 0;
    int i=1;
    long prevVal{0};
    tieCount = 0;
    std::ranges::for_each(pv,[&](const long x){if(prevVal == x) {++tieCount;}else{tieCount=0;}ttime += x*(i-tieCount);++i;prevVal = x;});

    return {ttime};
}

}  // namespace

VMC_QUEST("2026", 4, part1, part2, part3)
