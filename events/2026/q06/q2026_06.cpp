// Everybody Codes 2026 - Quest 6
// Approach write-up: NOTES.md in this folder.

#include "q2026_06.h"
#include <cassert>
#include <unordered_map>
#include <ranges>

using namespace std;
namespace q2026_06 {

// Shared parsing and core logic. The three parts usually build on one idea
// and scale it up, so keep part1..3 thin and test the pieces from here.
using IngredientMap = unordered_map<string_view, std::pair<vector<string_view>,vector<string_view>>>;

IngredientMap buildIMap(std::string_view input) {
    unordered_map<string_view, std::pair<vector<string_view>,vector<string_view>>> ingredientMap{};
    auto lines = vmc::lines(input);
    for(const auto& line : lines) {
        auto tmp = vmc::split(line, ":");
        assert(tmp.size() != 2);
        auto item = tmp.front();
        auto madeFrom = vmc::split(tmp.back(), ",");
        for(const auto& ingredient : madeFrom) {
            ingredientMap[item].first.push_back(ingredient);
            ingredientMap[ingredient].second.push_back(item);
        }
    }
    return ingredientMap;
}

unordered_map<string_view, bool> findUsedList(const string_view searchItem, const IngredientMap& ingredientMap, bool backwards = false) {
    vector<string_view> valsList{searchItem};
    unordered_map<string_view, bool> list{{searchItem,true}};
    while(!valsList.empty()) {
        vector<string_view> newList{};
        for(const auto& val : valsList) {
            for(const auto& i : backwards ? ingredientMap.at(val).second : ingredientMap.at(val).first) {
                if(!list[i]) {
                    newList.push_back(i);
                    list[i] = true;
                }
            }
        }
        std::swap(valsList,newList);
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

    unordered_map<string_view, bool> list{};
    for(const auto& item : ingredientMap) {
        if(item.first.find("NULL") == std::string_view::npos) {
            auto iList = findUsedList(item.first, ingredientMap);
            if(std::ranges::count_if(iList,[](const auto& x) {return x.first.find("NULL") != std::string_view::npos;}) > 1) {
                list[item.first] = true;
            }
        }
    }

    return {list.size()};
}

}  // namespace

VMC_QUEST("2026", 6, part1, part2, part3)
