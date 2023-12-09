#include "day09.h"

#include <iostream>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <numeric>

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

    using Input = std::vector<std::vector<int>>;

    Input processInput(const std::vector<std::string>& input)
    {
        Input res;

        for (auto line : input) {
            std::stringstream ss(line);
            std::vector<int> tempVec;

            int temp;
            while(ss >> temp) {
                tempVec.push_back(temp);
            }
            res.push_back(tempVec);
        }

        return res;
    }

    int process1(Input& input)
    {
        int res = 0;

        for (auto history : input) {
            std::vector<std::vector<int>> extrapolator = {history};

            while (std::accumulate(extrapolator.back().begin(), extrapolator.back().end(), 0)) {
                std::vector<int> temp;
                for (int i = 0; i < extrapolator.back().size() - 1; i++) {
                    temp.push_back(extrapolator.back()[i + 1] - extrapolator.back()[i]);
                }
                extrapolator.push_back(temp);
                if (history.size() - 1 == extrapolator.size()) {
                    break;
                }
            }

            for (int i = extrapolator.size() - 1; i > 0; i--) {
                extrapolator[i - 1].push_back(extrapolator[i].back() + extrapolator[i - 1].back());
            }

            res += extrapolator[0].back();
        }

        return res;
    }

    int process2(Input& input)
    {
        int res = 0;

        for (auto history : input) {
            std::vector<std::vector<int>> extrapolator = {history};

            while (std::accumulate(extrapolator.back().begin(), extrapolator.back().end(), 0)) {
                std::vector<int> temp;
                for (int i = 0; i < extrapolator.back().size() - 1; i++) {
                    temp.push_back(extrapolator.back()[i + 1] - extrapolator.back()[i]);
                }
                extrapolator.push_back(temp);
                if (history.size() - 1 == extrapolator.size()) {
                    break;
                }
            }

            int current = 0;
            for (int i = extrapolator.size() - 1; i > 0; i--) {
                current = extrapolator[i - 1].front() - current;
                // extrapolator[i - 1].push_back(extrapolator[i].back() + extrapolator[i - 1].back());
            }

            res += current;
        }

        return res;
    }
}

std::pair<int, int> day09()
{
    auto rawInput = readInput("inputs/day09.txt");
    auto input = processInput(rawInput);
    return {process1(input), process2(input)};
}