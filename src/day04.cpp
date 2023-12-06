#include "day04.h"

#include <iostream>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>

#include <numeric>

#include "util.h"


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

    std::vector<std::pair<std::set<int>, std::vector<int>>> processInput(const std::vector<std::string>& input)
    {
        std::vector<std::pair<std::set<int>, std::vector<int>>> res;

        for (auto line : input) {
            line = line.substr(line.find(": ") + 2);
            auto sets = split(line, " | ");

            const int winningLen = 10;
            std::stringstream winning(sets[0]);
            std::set<int> tempSet;
            for (int i = 0; i < winningLen; i++) {
                int temp;
                winning >> temp;
                tempSet.insert(temp);
            }

            const int potentialLen = 25;
            std::stringstream potential(sets[1]);
            std::vector<int> tempVector;
            for (int i = 0; i < potentialLen; i++) {
                int temp;
                potential >> temp;
                tempVector.push_back(temp);
            }

            res.push_back({tempSet, tempVector});
        }

        return res;
    }

    int process1(const std::vector<std::pair<std::set<int>, std::vector<int>>>& input)
    {
        int res = 0;

        for (auto& card : input) {
            auto& winning = card.first;
            const auto& potential = card.second;

            int count = 0;
            for (auto num : potential) {
                if (winning.count(num)) {
                    count++;
                }
            }

            if (count) {
                res += (1 << (count - 1));
            }
        }

        return res;
    }

    int process2(const std::vector<std::pair<std::set<int>, std::vector<int>>>& input)
    {
        int res = 0;

        std::vector<int> cards(input.size(), 1);
        for (int i = 0; i < input.size(); i++) {
            auto& winning = input[i].first;
            const auto& potential = input[i].second;

            int count = 0;
            for (auto num : potential) {
                if (winning.count(num)) {
                    count++;
                }
            }

            for (int j = i + 1; j < i + count + 1 && j < input.size(); j++) {
                cards[j] += cards[i];
            }
        }

        return std::accumulate(cards.begin(), cards.end(), 0);
    }
}

std::pair<int, int> day04()
{
    auto rawInput = readInput("inputs/day04.txt");
    auto input = processInput(rawInput);
    return {process1(input), process2(input)};
}