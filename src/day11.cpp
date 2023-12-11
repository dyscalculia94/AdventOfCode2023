#include "day11.h"

#include <iostream>

#include <fstream>
#include <string>
#include <vector>

namespace
{
    std::vector<int> emptyRows;
    std::vector<int> emptyCols;

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

    using Input = std::vector<std::pair<int, int>>;

    Input processInput(const std::vector<std::string>& input)
    {
        Input res;
        
        int count = 0;
        for (int i = 0; i < input[0].size(); i++) {
            bool isEmpty = true;
            for (int j = 0; j < input.size(); j++) {
                if (input[j][i] == '#') {
                    isEmpty = false;
                }
            }

            if (isEmpty) {
                count++;
            }
            emptyCols.push_back(count);
        }        

        count = 0;
        for (int i = 0; i < input.size(); i++) {
            bool isEmpty = true;
            for (int j = 0; j < input[i].size(); j++) {
                if (input[i][j] == '#') {
                    isEmpty = false;
                    res.push_back({i, j});
                }
            }

            if (isEmpty) {
                count++;
            }
            emptyRows.push_back(count);
        }

        return res;
    }

    void insertSpace(std::pair<int, int>& galaxy, int scale)
    {
        galaxy.first += emptyRows[galaxy.first] * scale;
        galaxy.second += emptyCols[galaxy.second] * scale;
    }

    int diff(std::pair<int, int> a, std::pair<int, int> b, int scale) {
        insertSpace(a, scale);
        insertSpace(b, scale);
        return std::abs(a.first - b.first) + std::abs(a.second - b.second);
    }

    long long process(const Input& input, int scale)
    {
        long long res = 0;

        for (int i = 0; i < input.size() - 1; i++) {
            for (int j = i + 1; j < input.size(); j++) {
                res += diff(input[i], input[j], scale);
            }
        }

        return res;
    }

    long long process1(const Input& input)
    {
        return process(input, 1);
    }

    long long process2(const Input& input)
    {
        return process(input, 999999);
    }
}

std::pair<long long, long long> day11()
{
    auto rawInput = readInput("inputs/day11.txt");
    auto input = processInput(rawInput);
    return {process1(input), process2(input)};
}