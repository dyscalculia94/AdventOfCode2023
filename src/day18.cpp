#include "day18.h"

#include <iostream>

#include <fstream>
#include <sstream>
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

    struct Dig
    {
        char dir;
        int len;
        std::string colour;
    };

    using Input = std::vector<Dig>;

    Input processInput(const std::vector<std::string>& input)
    {
        Input res;

        for (auto line : input) {
            std::stringstream ss(line);
            Dig d;
            ss >> d.dir >> d.len >> d.colour;
            d.colour = d.colour.substr(2, d.colour.size() - 2);
            res.push_back(d);
        }

        return res;
    }

    enum class Part
    {
        one,
        two
    };

    long long getLen(const Dig& dig, Part part)
    {
        return part == Part::one ? dig.len : std::stoll(dig.colour.substr(0, 5), nullptr, 16);
    }

    int getDir(const Dig& dig, Part part)
    {
        if (part == Part::one) {
            return (dig.dir == 'R' || dig.dir == 'D') ? 1 : -1;
        }
        else {
            return (dig.colour[5] == '0' || dig.colour[5] == '1') ? 1 : -1;
        }
    }

    bool isHorizontal(const Dig& dig, Part part)
    {
        if (part == Part::one) {
            return dig.dir == 'R' || dig.dir == 'L';
        }
        else {
            return dig.colour[5] == '0' || dig.colour[5] == '2';
        }
    }

    long long process(const Input& input, Part part)
    {
        long long res = 0;

        long long prevx = 0, prevy = 0;
        long long curx = 0, cury = 0;
        long long perimeter = 0;
        for (auto dig : input) {
            int dir = getDir(dig, part);
            long long len = getLen(dig, part);
            if (isHorizontal(dig, part)) {
                cury += len * dir;
            }
            else {
                curx += len * dir;
            }

            res += curx * prevy - cury * prevx;
            perimeter += len;
            prevx = curx;
            prevy = cury;
        }

        res = std::abs(res / 2) + perimeter / 2 + 1;

        return res;
    }

    long long process1(const Input& input)
    {
        return process(input, Part::one);
    }

    long long process2(const Input& input)
    {
        return process(input, Part::two);
    }
}

std::pair<long long, long long> day18()
{
    auto rawInput = readInput("inputs/day18.txt");
    auto input = processInput(rawInput);
    return {process1(input), process2(input)};
}