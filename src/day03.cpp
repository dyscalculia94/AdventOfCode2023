#include "day03.h"

#include <iostream>

#include <fstream>
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

    bool isSymbol(char c) {
        return !isdigit(c) && c != '.';
    }

    bool isValid(const std::vector<std::string>& input, int i, int j)
    {
        return i >= 0 && i < input.size() && j >= 0 && j < input[i].size();
    }

    bool isAdjacent(const std::vector<std::string>& input, int row, int start, int end)
    {
        for (int i = row - 1; i < row + 2; i += 2) {
            for (int j = start - 1; j < end + 2; j++) {
                if (isValid(input, i, j) && isSymbol(input[i][j])) {
                    return true;
                }
            }
        }

        if (isValid(input, row, start - 1) && isSymbol(input[row][start - 1])) {
            return true;
        }

        if (isValid(input, row, end + 1) && isSymbol(input[row][end + 1])) {
            return true;
        }

        return false;
    }

    int partNumber(const std::vector<std::string>& input, int row, int start, int end)
    {
        int num = 0;
        for (int i = start; i <= end; i++) {
            num = num * 10 + input[row][i] - '0';
        }

        return isAdjacent(input, row, start, end) ? num : 0;
    }

    int getNumberStart(const std::vector<std::string>& input, int row, int col)
    {
        while (isValid(input, row, col - 1) && isdigit(input[row][col - 1])) {
            col--;
        }

        return col;
    }

    int getNumber(const std::vector<std::string>& input, int row, int col)
    {
        int num = 0;
        while (isValid(input, row, col) && isdigit(input[row][col])) {
            num = num * 10 + input[row][col] - '0';
            col++;
        }

        return num;
    }

    std::vector<int> getAdjacentNumbers(const std::vector<std::string>& input, int row, int col)
    {
        std::vector<int> res;

        if (isValid(input, row, col - 1) && isdigit(input[row][col - 1])) {
            int start = getNumberStart(input, row, col - 1);
            res.push_back(getNumber(input, row, start));
        }
        if (isValid(input, row, col + 1) && isdigit(input[row][col + 1])) {
            res.push_back(getNumber(input, row, col + 1));
        }

        for (int i = row - 1; i < row + 2; i += 2) {
            std::vector<int> starts;
            for (int j = col - 1; j < col + 2; j++) {
                if (isValid(input, i, j) && isdigit(input[i][j])) {
                    int start = getNumberStart(input, i, j);
                    if (std::find(starts.begin(), starts.end(), start) == starts.end()) {
                        res.push_back(getNumber(input, i, start));
                        starts.push_back(start);
                    }
                }
            }
        }

        return res;
    }

    int process1(const std::vector<std::string>& input)
    {
        int res = 0;
        
        for (int i = 0; i < input.size(); i++) {
            int j = 0;
            while (j < input[i].size()) {
                if (isdigit(input[i][j])) {
                    int start = j;
                    while (j < input[i].size() && isdigit(input[i][j])) {
                        j++;
                    }
                    j--;
                    res += partNumber(input, i, start, j);
                }
                j++;
            }
        }

        return res;
    }

    int process2(const std::vector<std::string>& input)
    {
        long long res = 0;
        
        for (int i = 0; i < input.size(); i++) {
            for (int j = 0; j < input[i].size(); j++) {
                if (input[i][j] == '*') {
                    auto adjacent = getAdjacentNumbers(input, i, j);
                    if (adjacent.size() == 2) {
                        res += adjacent[0] * adjacent[1];
                    }
                }
            }
        }

        return res;
    }
}

std::pair<int, int> day03()
{
    auto input = readInput("inputs/day03.txt");

    return {process1(input), process2(input)};
}