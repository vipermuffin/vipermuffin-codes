// vmc                          list registered quests and which notes you have
// vmc <event> <quest>          run every part that has notes
// vmc <event> <quest> <part>   run one part
// vmc <event> <quest> <part> -f <file>   run one part on another file

#include <chrono>
#include <cstdio>
#include <exception>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "vmc/vmc.h"

namespace {

int usage() {
    std::cerr << "usage: vmc [<event> <quest> [<part>] [-f <file>]]\n"
                 "  e.g. vmc 2026 3      vmc 2026 3 2      vmc sample 0\n";
    return 2;
}

void listQuests() {
    if (vmc::registry().empty()) {
        std::cout << "No quests yet. Create one with: tools/new_quest.py <event> <quest>\n";
        return;
    }
    for (const auto& [key, q] : vmc::registry()) {
        std::cout << q.event << " q" << (q.number < 10 ? "0" : "") << q.number << "  notes:";
        for (int p = 1; p <= 3; ++p)
            std::cout << ' ' << (std::filesystem::exists(q.notesPath(p)) ? std::to_string(p) : "-");
        std::cout << '\n';
    }
}

}  // namespace

int main(int argc, char** argv) {
    std::vector<std::string> args;
    std::optional<std::string> file;
    for (int i = 1; i < argc; ++i) {
        const std::string a = argv[i];
        if (a == "-f" || a == "--file") {
            if (++i >= argc) return usage();
            file = argv[i];
        } else if (a == "-h" || a == "--help") {
            return usage();
        } else {
            args.push_back(a);
        }
    }

    if (args.empty()) {
        listQuests();
        return 0;
    }
    if (args.size() < 2 || args.size() > 3) return usage();

    const std::string event = args[0];
    int quest = 0;
    std::vector<int> parts{1, 2, 3};
    try {
        quest = std::stoi(args[1]);
        if (args.size() == 3) parts = {std::stoi(args[2])};
    } catch (const std::exception&) {
        return usage();
    }
    if (file && parts.size() != 1) {
        std::cerr << "-f needs a specific part, e.g. vmc " << event << ' ' << quest << " 1 -f example.txt\n";
        return 2;
    }

    const auto* q = vmc::findQuest(event, quest);
    if (!q) {
        std::cerr << "Quest " << event << ' ' << quest
                  << " isn't built in. Create it with tools/new_quest.py, then rebuild.\n";
        return 1;
    }

    for (int part : parts) {
        if (part < 1 || part > 3) return usage();
        const auto input = file ? vmc::readFile(*file) : vmc::readFile(q->notesPath(part));
        std::cout << "Part " << part << ": ";
        if (!input) {
            if (file)
                std::cout << "can't read " << *file << '\n';
            else
                std::cout << "(no notes yet - tools/fetch.py " << event << ' ' << quest << ")\n";
            continue;
        }
        try {
            const auto start = std::chrono::steady_clock::now();
            const auto answer = vmc::solve(event, quest, part, *input);
            const std::chrono::duration<double, std::milli> ms = std::chrono::steady_clock::now() - start;
            char timing[32];
            std::snprintf(timing, sizeof timing, "%.2f ms", ms.count());
            std::cout << (answer.empty() ? "(not solved yet)" : answer.value) << "   [" << timing << "]\n";
        } catch (const std::exception& e) {
            std::cout << "error: " << e.what() << '\n';
        }
    }
    return 0;
}
