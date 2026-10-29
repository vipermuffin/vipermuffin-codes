// Everybody Codes 2026 - Quest 2
// Approach write-up: NOTES.md in this folder.

#include "q2026_02.h"
#include <map>
#include <numeric>
#include <sstream> 
#include <vector>
#include <iostream>
#include <algorithm>
#include <ranges>

namespace q2026_02 {
    template <class T>
    void printVector(const std::vector<T>& v) {
        std::cout << "v = {";
        auto x  = v.begin();
        if(x>= v.end()) {
            std::cout << "}";
        }
        while(x < v.end()) {
            std::cout << *x++ << (x==v.end() ? "}" : ",");
        }
        std::cout << std::endl;
    }

    struct Alloy {
        Alloy() = delete;
        Alloy(int val, int max_val) {
            _val = val;
            comp_vals.resize(max_val, 0);
            comp_vals[val-1] = -2;
        }

        bool hasComparison(const int compVal) const {
            return compVal > 0 && static_cast<size_t>(compVal-1) < comp_vals.size() && comp_vals[compVal-1] != 0;
        }

        bool isComplete() const {
            return std::ranges::none_of(comp_vals,[](const int x ){return x==0;});
        }

        bool operator<(const int compVal) const {
            return comp_vals[compVal-1] == -1;
        }

        bool operator<(const Alloy& compAlloy) const {
            return *this < compAlloy.val();
        }

        bool operator>(const int compVal) const {
            return comp_vals[compVal-1] == 1;
        }

        bool operator>(const Alloy& compAlloy) const {
            return *this > compAlloy.val();
        }

        void setLessThan(const int x) {
            comp_vals[x-1] = -1;
        }

        void setGreaterThan(const int x) {
            comp_vals[x-1] = 1;
        }
        int greaterThanCount() const {
            return std::ranges::count(comp_vals,1);
        }
        std::ranges::view auto getLessThansView() const {
            return std::views::iota(1,static_cast<int>(comp_vals.size()+1)) | std::views::filter([this](int x) {return this->comp_vals[x-1] == -1;});
        }

        int val() const {
            return _val;
        }
        private:
        int _val;
        std::vector<int> comp_vals;
    };

int getChecksum(const std::string_view grp) {
    auto vals = vmc::ints<int>(grp);
    auto max_val = std::ranges::max(vals);
    //search for missing vals
    std::map<int,int> valCount{};
    for(const auto v : vals) {
        valCount[v]++;
    }
    std::vector<int> missing{};
    for(const auto& kvp : valCount) {
        if(kvp.second != max_val-1) {
            missing.push_back(kvp.first);
        }
    }
    std::vector<Alloy> compMap;
    compMap.reserve(max_val);
    for(int i = 1; i <= max_val;++i){
        compMap.emplace_back(i,max_val);
    }

    auto comps = vmc::split(grp, " ");
    for(const auto comp : comps) {
        auto nums = vmc::ints<int>(comp);
        if(comp.find(">") != std::string_view::npos) {
            compMap[nums[0]-1].setGreaterThan(nums[1]);
            compMap[nums[1]-1].setLessThan(nums[0]);
        } else {
            compMap[nums[0]-1].setLessThan(nums[1]);
            compMap[nums[1]-1].setGreaterThan(nums[0]);
        }
    }

    while(!missing.empty()) {
        std::vector<int> newMissing{};
        auto itr = missing.begin();
        bool updated = false;
        while(itr != missing.end()) {
            auto& m1 = compMap[*itr-1];
            itr++;
            
            auto itr2 = compMap.begin();
            while(itr2 != compMap.end()) {
                auto& m2 = *itr2;
                itr2++;
                if(!m1.hasComparison(m2.val())) {
                    for(const auto y : m1.getLessThansView()) {
                        if(m2.val() != y && m2.hasComparison(y) && m2>y) {
                            m2.setGreaterThan(m1.val());
                            m1.setLessThan(m2.val());
                            updated = true;
                        }
                    }
                    for(const auto y : m2.getLessThansView()) {
                        if(m1.val() != y && m1.hasComparison(y) && m1>y) {
                            m1.setGreaterThan(m2.val());
                            m2.setLessThan(m1.val());
                            updated = true;
                        }
                    }
                }
            }
        }
        if(updated) {
            for(const auto x : missing) {
                if(!compMap[x-1].isComplete()) {
                    newMissing.push_back(x);
                }
            }
        }

        std::swap(missing,newMissing);
    }

    std::ranges::sort(compMap,std::greater{});
    std::stringstream ss{};
    auto itr = compMap.begin();
    while(itr != compMap.end()) {
        ss << itr->val();
        itr++;
    }
    return std::stoi(ss.str());
}

}  // namespace q2026_02

namespace {

using namespace q2026_02;

vmc::Answer part1(std::string_view input) {
    long checksum{0};
    for(const auto line : vmc::lines(input)) {
        checksum += getChecksum(line);
    }
    return std::to_string(checksum);
}

vmc::Answer part2([[maybe_unused]] std::string_view input) {
    long checksum{0};
    for(const auto line : vmc::lines(input)) {
        checksum += getChecksum(line);
    }
    return std::to_string(checksum);
}

vmc::Answer part3([[maybe_unused]] std::string_view input) {
    long checksum{0};
    for(const auto line : vmc::lines(input)) {
        checksum += getChecksum(line);
    }
    return std::to_string(checksum);
}

}  // namespace

VMC_QUEST("2026", 2, part1, part2, part3)
