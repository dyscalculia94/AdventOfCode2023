#include "day21.h"

#include <iostream>

#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>

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

    using Input = std::vector<std::string>;

    struct Point
    {
        int i;
        int j;

        friend Point operator+(const Point& a, const Point& b) { return {a.i + b.i, a.j + b.j}; }
        friend bool operator==(const Point& a, const Point& b) { return a.i == b.i && a.j == b.j; }
    };

    Input processInput(const std::vector<std::string>& input)
    {
        return input;
    }

    bool isValid(const Input& input, Point p)
    {
        const int height = input.size();
        const int width = input.front().size();
        Point moved = {(p.i % height + height) % height, (p.j % width + width) % width};
        return input[moved.i][moved.j] != '#';
    }

    template<typename SetType>
    void processStep(const Input& input, SetType& current, Point p)
    {
        if (!isValid(input, p)) {
            return;
        }

        current.insert(p);
    }

    long long process1(const Input& input, int steps)
    {
        Point start;
        for (int i = 0; i < input.size(); i++) {
            for (int j = 0; j < input[i].size(); j++) {
                if (input[i][j] == 'S') {
                    start = {i, j};
                }
            }
        }

        std::vector<Point> dirs = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

        auto hash = [](const Point& p) { return 1000 * p.i + p.j; };
        std::unordered_set<Point, decltype(hash)> current(0, hash);
        std::unordered_set<Point, decltype(hash)> prev(0, hash);
        
        prev.insert(start);
        for (int i = 0; i < steps; i++) {
            for (auto p : prev) {
                for (auto dir : dirs) {
                    processStep(input, current, p + dir);
                }
            }

            std::swap(current, prev);
            current.clear();
        }

        return prev.size();
    }

    long long process2(const Input& input)
    {
        long long steps = 26501365;

        long long a0 = process1(input, 0 * 131 + 65);
        long long a1 = process1(input, 1 * 131 + 65);
        long long a2 = process1(input, 2 * 131 + 65);

        long long b0 = a0;
        long long b1 = a1 - a0;
        long long b2 = a2 - a1;

        long long x = steps / 131;

        return ((b0 + b1 * x + (x * (x - 1) / 2) * (b2 - b1)));
    }
}

std::pair<long long, long long> day21()
{
    auto rawInput = readInput("inputs/day21.txt");
    auto input = processInput(rawInput);
    return {process1(input, 64), process2(input)};
}