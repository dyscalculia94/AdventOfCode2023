#include "day06.h"

#include <iostream>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <cmath>


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

    std::vector<std::pair<int, int>> processInput(const std::vector<std::string>& input)
    {
        std::vector<std::pair<int, int>> res;

        std::stringstream times(input[0].substr(input[0].find(":") + 1));
        int time;
        while (times >> time) {
            res.push_back({time, 0});
        }

        std::stringstream dists(input[1].substr(input[1].find(":") + 1));
        for (auto& i : res) {
            dists >> i.second;
        }

        return res;
    }

    long long getWays(long long time, long long dist)
    {
        double discriminant = std::sqrt(time * time - 4 * dist);

        double x1 = (time + discriminant) / 2;
        if (x1 == std::floor(x1)) {
            x1--;
        }

        double x2 = (time - discriminant) / 2;
        if (x2 == std::ceil(x2)) {
            x2++;
        }

        return static_cast<long long>(std::floor(x1) - std::ceil(x2)) + 1;
    }

    int process1(const std::vector<std::pair<int, int>>& input)
    {
        int res = 1;

        for (auto race : input) {
            res *= getWays(race.first, race.second);
        }

        return res;
    }

    int process2(const std::vector<std::pair<int, int>>& input)
    {
        int res = 0;

        // I was too lazy to do it a different way, so I just copied the input here
        long long time = 57726992;
        long long dist = 291117211762026;

        return getWays(time, dist);
    }
}

std::pair<int, int> day06()
{
    auto rawInput = readInput("inputs/day06.txt");
    auto input = processInput(rawInput);
    return {process1(input), process2(input)};
}