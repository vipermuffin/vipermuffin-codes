#pragma once
// One include for quest files. Add your own helpers (search, math, etc.)
// as new headers next to these and include them here.

#include "vmc/answer.h"
#include "vmc/combinations.h"
#include "vmc/grid.h"
#include "vmc/parse.h"
#include "vmc/registry.h"

#include <iostream>
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