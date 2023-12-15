#include "day15.h"

#include <iostream>

#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>

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

    using Input = std::vector<std::string>;

    Input processInput(const std::vector<std::string>& input)
    {
        return split(input.front(), ",");
    }

    int getHash(const std::string& step)
    {
        int res = 0;

        for (auto c : step) {
            res += c;
            res *= 17;
            res %= 256;
        }

        return res;
    }

    std::string getLabel(const std::string& step, size_t position)
    {
        return step.substr(0, position);
    }
    
    std::pair<std::string, int> getLabelAndNumber(const std::string& step, size_t position)
    {
        return {getLabel(step, position), std::stoi(step.substr(position + 1))};
    }

    int process1(Input input)
    {
        int res = 0;

        for (auto step : input) {
            res += getHash(step);
        }

        return res;
    }

    int process2(Input input)
    {
        int res = 0;

        std::map<int, std::list<std::pair<std::string, int>>> boxes;
        for (auto step : input) {
            auto equals = step.find('=');
            auto dash = step.find('-');

            if (equals != std::string::npos) {
                auto args = getLabelAndNumber(step, equals);
                int hash = getHash(args.first);

                auto box = boxes.find(hash);
                if (box != boxes.end()) {
                    auto& list = box->second;
                    bool found = false;
                    for (auto& label : list) {
                        if (label.first == args.first) {
                            found = true;
                            label.second = args.second;
                            break;
                        }
                    }
                    if (!found) {
                        list.push_back(args);
                    }
                }
                else {
                    boxes[hash] = {args};
                }
            }
            else {
                auto arg = getLabel(step, dash);
                int hash = getHash(arg);

                auto box = boxes.find(hash);
                if (box != boxes.end()) {
                    auto& list = box->second;

                    list.remove_if([arg](auto label) {
                        return label.first == arg;
                    });
                }
            }
        }

        for (auto box : boxes) {
            const int boxNum = box.first + 1;
            int i = 1;
            for (auto lens : box.second) {
                res += boxNum * i * lens.second;
                i++;
            }
        }

        return res;
    }
}

std::pair<int, int> day15()
{
    auto rawInput = readInput("inputs/day15.txt");
    auto input = processInput(rawInput);
    return {process1(input), process2(input)};
}