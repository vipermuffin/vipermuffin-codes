// Everybody Codes 2026 - Quest 5
// Approach write-up: NOTES.md in this folder.

#include "q2026_05.h"
#include <iostream>
#include <vector>
#include <unordered_set>

namespace q2026_05 {

// Shared parsing and core logic. The three parts usually build on one idea
// and scale it up, so keep part1..3 thin and test the pieces from here.
void findLandCellsAndUpdateMap(vmc::CharGrid& landMap, const vmc::Point start) {
    auto points2Review = landMap.neighbours8(start);
    while(!points2Review.empty()) {
        std::unordered_set<vmc::Point> newPoints{};
        std::ranges::for_each(points2Review, [&](const auto p) {
            auto count = std::ranges::count_if(landMap.neighbours8(p),[&](const auto p2){return landMap[p2] == '#' || landMap[p2] == '1';});
            if(count >= 5){
                landMap[p]='#';
                std::ranges::for_each(landMap.neighbours8(p),[&](const auto p2){if(landMap[p2] != '#') {newPoints.insert(p2);}});
            }
        });
        std::vector<vmc::Point> newPV{};
        std::ranges::for_each(newPoints,[&](const auto p){newPV.push_back(p);});
        // std::cout << newPoints.size() << std::endl;
        std::swap(points2Review,newPV);
    }
}

void findWaterCellsAndUpdateMap(vmc::CharGrid& landMap) {
    std::ranges::for_each(landMap.points(),[&](const auto p){if(landMap[p] != '#') { landMap[p] = '.';}});
}

void findSandCellsAndUpdateMap(vmc::CharGrid& landMap) {
    
    //First get outside edge
    std::ranges::for_each(landMap.findAll('#'), [&](const auto p) {
        auto neighbors = landMap.neighbours8(p);
        if(neighbors.size() == 8) {
            auto count = std::ranges::count_if(neighbors,[&](const auto p2){return landMap[p2] == '0' || landMap[p2] == '1' || landMap[p2] == '.';});
            if(count > 0){
                landMap[p]='=';
            }
        } else {
            landMap[p]='=';
        }
    });

    //Now count neighboring sand
    auto points2Review = landMap.findAll('#');
    while(!points2Review.empty()) {
        std::unordered_set<vmc::Point> newPoints{};
        std::ranges::for_each(points2Review, [&](const auto p) {
            auto neighbors = landMap.neighbours8(p);
            bool isSand = false;
            if(neighbors.size() == 8) {
                auto count = std::ranges::count_if(neighbors,[&](const auto p2){return landMap[p2] == '=';});
                if(count >= 5){
                    isSand = true;
                }
            } else {
                isSand = true;
            }
            if(isSand){
                landMap[p]='=';
                std::ranges::for_each(landMap.neighbours8(p),[&](const auto p2){if(landMap[p2] == '#') {newPoints.insert(p2);}});
            }
        });
        std::vector<vmc::Point> newPV{};
        std::ranges::for_each(newPoints,[&](const auto p){newPV.push_back(p);});
        // std::cout << newPoints.size() << std::endl;
        std::swap(points2Review,newPV);
    }
}

void initTrees(vmc::CharGrid& landMap) {
    std::vector<vmc::Point> points2Review = landMap.findAll('#');
    std::ranges::for_each(points2Review,[&](const auto p){
        if((p.r > 1) && (p.r < (landMap.rows()-1)) && (p.c > 1) && (p.c < landMap.cols()-1)) {
            if(std::ranges::all_of(landMap.neighbours8(p),[&](const auto p2){return (landMap[p2] == '#' || landMap[p2] == '@') && std::ranges::all_of(landMap.neighbours4(p2),[&](const auto p3){return landMap[p3] == '#' || landMap[p3] == '@';});})) {
                landMap[p] = '@';
            }
        }
    });
}

void growTrees(vmc::CharGrid& landMap) {
    std::vector<vmc::Point> points2Review = landMap.findAll('#');
    std::vector<vmc::Point> points2Update{};
    std::ranges::for_each(points2Review,[&](const auto p){
        auto neighbors = landMap.neighbours8(p);
        if(std::ranges::none_of(neighbors,[&](const auto p2) {return landMap[p2]=='=';})) {
            if(std::ranges::count_if(neighbors,[&](const auto p2){return landMap[p2]=='@';}) >= 4) {
                points2Update.push_back(p);
            }
        }
    });
    std::ranges::for_each(points2Update,[&](const auto p){landMap[p] = '@';});
}
}  // namespace q2026_05

namespace {

using namespace q2026_05;

vmc::Answer part1([[maybe_unused]] std::string_view input) {
    vmc::CharGrid landMap = vmc::parseGrid(input);
    vmc::Point mapCenter{};
    mapCenter.r = landMap.rows()/2;
    mapCenter.c = landMap.cols()/2;

    landMap[mapCenter] = '#';
    findLandCellsAndUpdateMap(landMap, mapCenter);
    // auto point2Review = landMap.neighbours8(mapCenter);
    // while(!point2Review.empty()) {
    //     //std::vector<vmc::Point> newPoints{};
    //     std::unordered_set<vmc::Point> newPoints{};
    //     std::ranges::for_each(point2Review, [&](const auto p) {
    //         auto count = std::ranges::count_if(landMap.neighbours8(p),[&](const auto p2){return landMap[p2] == '#' || landMap[p2] == '1';});
    //         if(count >= 5){
    //             landMap[p]='#';
    //             std::ranges::for_each(landMap.neighbours8(p),[&](const auto p2){if(landMap[p2] != '#') {newPoints.insert(p2);}});
    //         }
    //     });
    //     std::vector<vmc::Point> newPV{};
    //     std::ranges::for_each(newPoints,[&](const auto p){newPV.push_back(p);});
    //     // std::cout << newPoints.size() << std::endl;
    //     std::swap(point2Review,newPV);
    // }
    findWaterCellsAndUpdateMap(landMap);
    return {std::ranges::count_if(landMap.points(),[&](const auto p){return landMap[p] == '#';})};
}

vmc::Answer part2([[maybe_unused]] std::string_view input) {
    vmc::CharGrid landMap = vmc::parseGrid(input);
    vmc::Point mapCenter{};
    mapCenter.r = landMap.rows()/2;
    mapCenter.c = landMap.cols()/2;

    landMap[mapCenter] = '#';
    findLandCellsAndUpdateMap(landMap, mapCenter);
    findSandCellsAndUpdateMap(landMap);
    return {std::ranges::count_if(landMap.points(),[&](const auto p){return landMap[p] == '=';})};
}

vmc::Answer part3([[maybe_unused]] std::string_view input) {
    vmc::CharGrid landMap = vmc::parseGrid(input);
    vmc::Point mapCenter{};
    mapCenter.r = landMap.rows()/2;
    mapCenter.c = landMap.cols()/2;

    landMap[mapCenter] = '#';
    findLandCellsAndUpdateMap(landMap, mapCenter);
    findSandCellsAndUpdateMap(landMap);
    initTrees(landMap);
    growTrees(landMap);
    growTrees(landMap);
    return {std::ranges::count_if(landMap.points(),[&](const auto p){return landMap[p] == '@';})};
}

}  // namespace

VMC_QUEST("2026", 5, part1, part2, part3)
