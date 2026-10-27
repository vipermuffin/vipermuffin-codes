// Everybody Codes 2025 - Quest 2
// Approach write-up: NOTES.md in this folder.

#include "q2025_02.h"
#include <cstdio>
#include <iostream>

namespace q2025_02 {

// Shared parsing and core logic. The three parts usually build on one idea
// and scale it up, so keep part1..3 thin and test the pieces from here.

bool shouldEngrave(const EC_Complex<long>& p) {
    constexpr long kLimit = 1000000;
    const EC_Complex<long> divisor(100000, 100000);
    EC_Complex<long> r(0, 0);
    for(int i = 0; i < 100; ++i) {
        r = performCycle(r, divisor, p);
        if(r.x < -kLimit || r.x > kLimit || r.y < -kLimit || r.y > kLimit) {
            return false;
        }
    }
    return true;
}

int countEngravedPointsWithResolution(int x, int y, int step) {
    int engrave_count = 0;
    for(int dy = 0; dy <= 1000; dy += step) {
        for(int dx = 0; dx <= 1000; dx += step) {
            if(shouldEngrave({x + dx, y + dy})) {
                ++engrave_count;
            }
        }
    }
    return engrave_count;
}

}  // namespace q2025_02

namespace {

using namespace q2025_02;

vmc::Answer part1([[maybe_unused]] std::string_view input) {
    int x{}, y{};
    std::string line{input};
    EC_Complex<int> r(0, 0);
    if(sscanf(line.c_str(), "A=[%d,%d]", &x, &y) == 2) {
        EC_Complex<int> a(x, y);
        for(int i = 0; i < 3; ++i) {
            r = r * r;
            r = r / EC_Complex<int>(10, 10);
            r = r + a;
        }
        return {r.str()};
    }
    std::cout << "Invalid input: " << line << std::endl;

    return {};
}

vmc::Answer part2([[maybe_unused]] std::string_view input) {
    int x{}, y{};
    std::string line{input};
    
    int engrave_count = 0;
    if(sscanf(line.c_str(), "A=[%d,%d]", &x, &y) == 2) {
        for(int dy = 0; dy <= 1000; dy += 10) {
            for(int dx = 0; dx <= 1000; dx += 10) {
                bool engrave = true;
                EC_Complex<long> r(0, 0);
                EC_Complex<long> p(x + dx, y + dy);
                for(int i = 0; (i < 100) && engrave; ++i ) {
                    r = r * r;
                    r = r / EC_Complex<long>(100000, 100000);
                    r = r + p;
                    if(r.x < -1000000 || r.x > 1000000 || r.y < -1000000 || r.y > 1000000) {
                        engrave = false;
                    }
                }
                if(engrave) {
                    ++engrave_count;
                }
            }
        }
        return {std::to_string(engrave_count)};
    }
    std::cout << "Invalid input: " << line << std::endl;
    return {};
}

vmc::Answer part3([[maybe_unused]] std::string_view input) {
    int x{}, y{};
    std::string line{input};
    
    int engrave_count = 0;
    if(sscanf(line.c_str(), "A=[%d,%d]", &x, &y) == 2) {
        for(int dy = 0; dy <= 1000; dy += 1) {
            for(int dx = 0; dx <= 1000; dx += 1) {
                bool engrave = true;
                EC_Complex<long> r(0, 0);
                EC_Complex<long> p(x + dx, y + dy);
                for(int i = 0; (i < 100) && engrave; ++i ) {
                    r = r * r;
                    r = r / EC_Complex<long>(100000, 100000);
                    r = r + p;
                    if(r.x < -1000000 || r.x > 1000000 || r.y < -1000000 || r.y > 1000000) {
                        engrave = false;
                    }
                }
                if(engrave) {
                    ++engrave_count;
                }
            }
        }
        return {std::to_string(engrave_count)};
    }
    std::cout << "Invalid input: " << line << std::endl;
    return {};
}

}  // namespace

VMC_QUEST("2025", 2, part1, part2, part3)
