#include "day02.h"

#include <iostream>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "util.h"

namespace
{
    constexpr int maxRed = 12;
    constexpr int maxGreen = 13;
    constexpr int maxBlue = 14;

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

    struct RGB
    {
        RGB() : red(0), green(0), blue(0) { }

        int power() { return red * green * blue; }

        void max(const RGB& rgb) {
            red = std::max(red, rgb.red);
            green = std::max(green, rgb.green);
            blue = std::max(blue, rgb.blue);
        }

        int red;
        int green;
        int blue;
    };

    void fillColour(RGB& rgb, int num, std::string colour) {
        if (colour == "red") {
            rgb.red = num;
        }
        if (colour == "green") {
            rgb.green = num;
        }
        if (colour == "blue") {
            rgb.blue = num;
        }
    }

    std::vector<std::vector<RGB>> processInput(const std::vector<std::string>& input) {
        std::vector<std::vector<RGB>> processed;

        for (auto line : input) {
            line = line.substr(line.find(": ") + 2);
            auto sets = split(line, "; ");

            std::vector<RGB> rgbSets;
            for (auto set : sets) {
                auto picks = split(set, ", ");
                
                RGB temp;
                for (auto pick : picks) {
                    std::stringstream ss(pick);
                    int num;
                    std::string colour;
                    ss >> num >> colour;

                    fillColour(temp, num, colour);
                }
                rgbSets.push_back(temp);
            }

            processed.push_back(rgbSets);
        }

        return processed;
    }

    bool isValid(const RGB& rgb)
    {
        return rgb.red <= maxRed && rgb.green <= maxGreen && rgb.blue <= maxBlue;
    }

    int process1(const std::vector<std::vector<RGB>>& input)
    {
        int res = 0;
        for (int i = 0; i < input.size(); i++) {
            bool valid = true;
            for (auto set : input[i]) {
                if (!isValid(set)) {
                    valid = false;
                    break;
                }
            }

            if (valid) {
                res += i + 1;
            }
        }

        return res;
    }

    int process2(const std::vector<std::vector<RGB>>& input)
    {
        int res = 0;
        for (int i = 0; i < input.size(); i++) {
            RGB max;
            for (auto set : input[i]) {
                max.max(set);
            }

            res += max.power();
        }

        return res;
    }
}

std::pair<int, int> day02()
{
    auto rawInput = readInput("inputs/day02.txt");
    auto input = processInput(rawInput);

    return {process1(input), process2(input)};
}