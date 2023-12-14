#include "day14.h"

#include <iostream>

#include <fstream>
#include <string>
#include <vector>
#include <map>

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
        return input;
    }

    Input tiltVertical(Input input, int dir)
    {
        const int startRow = dir == 1 ? 0 : input.size() - 1;
        const int endRow = dir == 1 ? input.size() : -1;
        const int startCol = dir == 1 ? 0 : input.front().size() - 1;
        const int endCol = dir == 1 ? input.front().size() : -1;

        for (int i = startCol; i != endCol; i += dir) {
            int current = startRow;
            for (int j = startRow; j != endRow; j += dir) {
                switch (input[j][i]) {
                    case '#':
                        current = j + dir;
                        break;
                    case 'O':
                        input[current][i] = 'O';
                        if (current != j) {
                            input[j][i] = '.';
                        }
                        current += dir;
                        while (current != endRow && input[current][i] == '#') {
                            current += dir;
                        }
                        break;
                    default:
                        break;
                }
            }
        }

        return input;
    }

    Input tiltHorizontal(Input input, int dir)
    {
        const int startRow = dir == 1 ? 0 : input.size() - 1;
        const int endRow = dir == 1 ? input.size() : -1;
        const int startCol = dir == 1 ? 0 : input.front().size() - 1;
        const int endCol = dir == 1 ? input.front().size() : -1;

        for (int i = startRow; i != endRow; i += dir) {
            int current = startCol;
            for (int j = startCol; j != endCol; j += dir) {
                switch (input[i][j]) {
                    case '#':
                        current = j + dir;
                        break;
                    case 'O':
                        input[i][current] = 'O';
                        if (current != j) {
                            input[i][j] = '.';
                        }
                        current += dir;
                        while (current != endCol && input[i][current] == '#') {
                            current += dir;
                        }
                        break;
                    default:
                        break;
                }
            }
        }

        return input;
    }

    Input tiltNorth(Input input)
    {
        return tiltVertical(input, 1);
    }

    Input tiltSouth(Input input)
    {
        return tiltVertical(input, -1);
    }

    Input tiltWest(Input input)
    {
        return tiltHorizontal(input, 1);
    }

    Input tiltEast(Input input)
    {
        return tiltHorizontal(input, -1);
    }

    int process1(Input input)
    {
        int res = 0;

        Input temp = input;

        input = tiltNorth(input);

        for (int i = 0; i < input.size(); i++) {
            int count = std::count(input[i].begin(), input[i].end(), 'O');
            res += (input.size() - i) * count;
        }

        return res;
    }

    int process2(Input input)
    {
        int res = 0;

        const int iterations = 1'000'000'000;
        const int start = 85;
        const int cycle = 22;
        const int first = (iterations - start) % cycle + start;
        for (int i = 0; i < iterations; i++) {
            input = tiltEast(tiltSouth(tiltWest(tiltNorth(input))));

            if (i == first) {
                break;
            }
        }

        for (int i = 0; i < input.size(); i++) {
            int count = std::count(input[i].begin(), input[i].end(), 'O');
            res += (input.size() - i) * count;
        }

        return res;
    }
}

std::pair<int, int> day14()
{
    auto rawInput = readInput("inputs/day14.txt");
    auto input = processInput(rawInput);
    return {process1(input), process2(input)};
}