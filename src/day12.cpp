#include "day12.h"

#include <iostream>

#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "util.h"

namespace
{
    std::map<std::pair<int, int>, long long> memo;

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

    struct Row
    {
        std::string row;
        std::vector<int> groups;
    };

    using Input = std::vector<Row>;

    Input processInput(const std::vector<std::string>& input)
    {
        Input res;
        
        for (auto line : input) {
            auto splitLine = split(line, " ");
            auto splitGroups = split(splitLine[1], ",");

            std::vector<int> groups;
            for (auto group : splitGroups) {
                groups.push_back(std::stoi(group));
            }

            res.push_back({splitLine[0], groups});
        }

        return res;
    }

    bool isGroupable(char c) {
        return c == '#' || c == '?';
    }

    long long check(Row row, int pos, int group)
    {
        if (pos == 0 && group == 0) {
            memo.clear();
        }

        if (memo.find({pos, group}) != memo.end()) {
            return memo[{pos, group}];
        }

        if (pos >= row.row.length()) {
            // std::cout << row.row << std::endl;
            memo[{pos, group}] = group == row.groups.size() ? 1 : 0;
            return memo[{pos, group}];
        }

        if (row.row[pos] == '.') {
            memo[{pos, group}] = check(row, pos + 1, group);
            return memo[{pos, group}];
        }
        else if (row.row[pos] == '?') {
            row.row[pos] = '.';
            auto res1 =  check(row, pos + 1, group);
            row.row[pos] = '#';
            auto res2 =  check(row, pos, group);
            memo[{pos, group}] = res1 + res2;
            return memo[{pos, group}];
        }
        else {
            if (group == row.groups.size()) {
                return 0;
            }

            int size = row.groups[group];
            int i = 0;
            while (i < size && (pos + i < row.row.length()) && isGroupable(row.row[pos + i])) {
                i++;
            }

            if (i < size) {
                memo[{pos, group}] = 0;
                return 0;
            }

            if ((pos + i < row.row.length()) && row.row[pos + i] == '#') {
                memo[{pos, group}] = 0;
                return 0;
            }

            memo[{pos, group}] = check(row, pos + i + 1, group + 1);
            return memo[{pos, group}];
        }
    }

    long long getNumberOfCombinations(const Row& row)
    {
        Row temp = row;
        return check(temp, 0, 0);
    }

    int process1(const Input& input)
    {
        int res = 0;

        for (auto row : input) {
            res += getNumberOfCombinations(row);
        }

        return res;
    }

    long long process2(const Input& input)
    {
        long long res = 0;

        int count = 0;
        for (auto row : input) {
            std::string tempRow = row.row;
            std::vector<int> tempGroups = row.groups;
            for (int i = 0; i < 4; i++) {
                row.row += "?" + tempRow;
                row.groups.insert(row.groups.end(), tempGroups.begin(), tempGroups.end());
            }

            res += getNumberOfCombinations(row);
        }
        
        return res;
    }
}

std::pair<int, long long> day12()
{
    auto rawInput = readInput("inputs/day12.txt");
    auto input = processInput(rawInput);
    return {process1(input), process2(input)};
}