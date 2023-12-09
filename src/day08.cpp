#include "day08.h"

#include <iostream>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>

#include <numeric>


namespace
{
    std::string instructions;
    const std::string start = "AAA";
    const std::string end = "ZZZ";

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

    using Input = std::unordered_map<std::string, std::pair<std::string, std::string>>;

    Input processInput(const std::vector<std::string>& input)
    {
        Input res;

        instructions = input[0];

        for (int i = 2; i < input.size(); i++) {
            std::string current = input[i];
            std::replace(current.begin(), current.end(), '=', ' ');
            std::replace(current.begin(), current.end(), '(', ' ');
            std::replace(current.begin(), current.end(), ')', ' ');
            std::replace(current.begin(), current.end(), ',', ' ');

            std::stringstream ss(current);
            std::string src, left, right;
            ss >> src >> left >> right;
            res[src] = {left, right};
        }

        return res;
    }

    int process1(Input& input)
    {
        int res = 0;

        // for (auto i : input) {
        //     std::cout << i.first << " " << i.second.first << " " << i.second.second << std::endl;
        // }

        // std::string next = start;
        // while (true) {
        //     for (auto i : instructions) {
        //         // std::cout << next << std::endl;
        //         if (next == end) {
        //             return res;
        //         }

        //         res++;

        //         next = (i == 'L' ? input[next].first : input[next].second);
        //     }
        // }
        return 0;
    }

    bool isDone(const std::vector<std::string>& next)
    {
        for (auto i : next) {
            if (i[2] != 'Z') {
                return false;
            }
        }

        return true;
    }

    int printCycle(Input& input, std::string s)
    {
        int res = 0;
        std::set<std::string> set;
        std::string next = s;
        set.insert(next);
        // std::cout << next;
        bool first = true;
        while (true) {
            for (auto i : instructions) {
                if (next[2] == 'Z') {
                    if (first) {
                        std::cout << res << " ";
                        first = false;
                        res = 0;
                    }             
                    else {
                        std::cout << res << std::endl;
                        return res;
                    }
                }
                res++;
                next = (i == 'L' ? input[next].first : input[next].second);
                set.insert(next);
                // std::cout << "->" << next;
            }
        }
    }

    long long process2(Input& input)
    {
        long long res = 0;

        std::vector<std::string> next;
        for (auto i : input) {
            if (i.first[2] == 'A') {
                next.push_back(i.first);
            }
        }

        std::vector<long long> counts;
        for (auto i : next) {
            counts.push_back(printCycle(input, i));
        }

        res = counts[0];
        for (int i = 1; i < counts.size(); i++) {
            res = std::lcm(res, counts[i]);
        }

        return res;
    }
}

std::pair<int, long long> day08()
{
    auto rawInput = readInput("inputs/day08.txt");
    auto input = processInput(rawInput);
    return {process1(input), process2(input)};
}