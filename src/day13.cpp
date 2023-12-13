#include "day13.h"

#include <iostream>

#include <fstream>
#include <string>
#include <vector>

namespace
{
    std::vector<std::string> readInput(std::string file)
    {
        std::ifstream in(file);

        std::vector<std::string> res;
        std::string temp;
        while (std::getline(in, temp)) {
            res.push_back(temp);
        }

        return res;
    }

    using Input = std::vector<std::vector<std::string>>;

    Input processInput(const std::vector<std::string>& input)
    {
        Input res;
        
        std::vector<std::string> pattern;
        for (auto line : input) {
            if (line.empty()) {
                res.push_back(pattern);
                pattern.clear();
            }
            else {
                pattern.push_back(line);
            }
        }
        if (!pattern.empty()) {
            res.push_back(pattern);
        }

        return res;
    }

    int compare_default(const std::string& a, const std::string& b)
    {
        return a == b;
    }

    int compare_smudge(const std::string& a, const std::string& b)
    {
        bool found = false;
        for (int i = 0; i < a.size(); i++) {
            if (a[i] != b[i]) {
                if (found) {
                    return 0;
                }
                else {
                    found = true;
                }
            }
        }

        return found ? -1 : 1;
    }

    template<typename Compare>
    bool isReflection(const std::vector<std::string>& pattern, int line, Compare& comp)
    {
        int up = line;
        int down = line + 1;
        bool found = false;
        while (up >= 0 && down < pattern.size()) {
            int diff = comp(pattern[up], pattern[down]);
            if (!diff) {
                return false;
            }
            if (diff == -1) {
                if (found) {
                    return false;
                }
                else {
                    found = true;
                }
            }
            up--;
            down++;
        }

        return comp == compare_default ? true : found;
    }

    template<typename Compare>
    int getLine(const std::vector<std::string>& pattern, Compare& comp)
    {
        for (int i = 0; i < pattern.size() - 1; i++) {
            if (isReflection(pattern, i, comp)) {
                return i + 1;
            }
        }

        return -1;
    }

    std::vector<std::string> flip(const std::vector<std::string>& pattern) {
        std::vector<std::string> res;

        for (int i = 0; i < pattern.front().size(); i++) {
            std::string temp;
            for (int j = 0; j < pattern.size(); j++) {
                temp += pattern[j][i];
            }
            res.push_back(temp);
        }

        return res;
    }

    template<typename Compare>
    int process(const Input& input, Compare& comp)
    {
        int res = 0;

        for (auto pattern : input) {
            auto vert = getLine(flip(pattern), comp);
            auto hor = getLine(pattern, comp);
            res += ((vert != -1) ? vert : 0) + 100 * ((hor != -1) ? hor : 0);
        }

        return res;
    }

    int process1(const Input& input)
    {
        return process(input, compare_default);
    }

    int process2(const Input& input)
    {
        return process(input, compare_smudge);
    }
}

std::pair<int, int> day13()
{
    auto rawInput = readInput("inputs/day13.txt");
    auto input = processInput(rawInput);
    return {process1(input), process2(input)};
}