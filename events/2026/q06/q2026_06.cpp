// Everybody Codes 2026 - Quest 6
// Approach write-up: NOTES.md in this folder.

#include "q2026_06.h"
#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <unordered_set>
#include <ranges>

namespace q2026_06 {

// Shared parsing and core logic. The three parts usually build on one idea
// and scale it up, so keep part1..3 thin and test the pieces from here.
using IngredientMap = std::unordered_map<std::string_view, std::pair<std::vector<std::string_view>,std::vector<std::string_view>>>;

IngredientMap buildIMap(std::string_view input) {
    IngredientMap ingredientMap{};
    auto lines = vmc::lines(input);
    for(const auto& line : lines) {
        auto tmp = vmc::split(line, ":");
        assert(tmp.size() == 2);
        auto item = tmp.front();
        auto madeFrom = vmc::split(tmp.back(), ",");
        for(const auto& ingredient : madeFrom) {
            ingredientMap[item].first.push_back(ingredient);
            ingredientMap[ingredient].second.push_back(item);
        }
    }
    return ingredientMap;
}

std::unordered_set<std::string_view> findUsedList(const std::string_view searchItem, const IngredientMap& ingredientMap, bool backwards = false) {
    std::vector<std::string_view> valsList{searchItem};
    std::unordered_set<std::string_view> list{searchItem};
    while(!valsList.empty()) {
        auto val = valsList.back();
        valsList.pop_back();
        const auto& [ingredients, usedIn] = ingredientMap.at(val);
        for(const auto next : backwards ? usedIn : ingredients) {
            if(list.insert(next).second) {
                valsList.push_back(next);
            }
        }
    }

    return list;
}

}  // namespace q2026_06

namespace {

using namespace q2026_06;


vmc::Answer part1(std::string_view input) {
    auto ingredientMap = buildIMap(input);

    auto list = findUsedList("NULL", ingredientMap);

    return {list.size()-1};
}

vmc::Answer part2(std::string_view input) {
    auto ingredientMap = buildIMap(input);

    auto list = findUsedList("NULL", ingredientMap, true);

    return {list.size()-1};
}

vmc::Answer part3(std::string_view input) {
    auto ingredientMap = buildIMap(input);

    std::unordered_map<std::string_view, int> badItems{};
    auto isNull = [](std::string_view k) { return k.starts_with("NULL");};
    auto nullVals = ingredientMap | std::views::keys | std::views::filter(isNull);
    for(const auto& item : nullVals) {
        auto iList = findUsedList(item, ingredientMap, true);
        for( auto x : iList) {++badItems[x];}
    }
    return {std::ranges::count_if(badItems,[&](const auto& entry){ const auto& [name, count] = entry; return !isNull(name) && count > 1;})};
}

}  // namespace

VMC_QUEST("2026", 6, part1, part2, part3)
