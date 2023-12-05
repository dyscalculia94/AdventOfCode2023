#include "day01.h"

#include <fstream>
#include <string>
#include <vector>
#include <array>

std::vector<std::string> readInput(std::string file)
{
    std::ifstream in(file);

    std::vector<std::string> res;
    std::string temp;
    while (in >> temp) {
        res.push_back(temp);
    }

    return res;
}

std::  array<std::string, 9> digits = {
    "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
};

int isSpelledNum(std::string line, int start)
{
    for (int i = 0; i < digits.size(); i++) {
        if (line.substr(start).find(digits[i]) == 0) {
            return i + 1;
        }
    }

    return -1;
}

int process1(std::string line)
{
    int number = 0;
    int i = 0;
    while (i < line.size() && !isdigit(line[i])) {
        i++;
    }
    number = line[i] - '0';

    i = line.size() - 1;
    while (i >= 0 && !isdigit(line[i])) {
        i--;
    }
    number = number * 10 + line[i] - '0';

    return number;
}

int process2(std::string line)
{
    int number = 0;
    int i = 0;
    int spell = isSpelledNum(line, 0);
    while (i < line.size() && !isdigit(line[i]) && spell == -1) {
        i++;
        spell = isSpelledNum(line, i);
    }
    number = (spell == -1 ? line[i] - '0' : spell);

    i = line.size() - 1;
    spell = -1;
    while (i >= 0 && !isdigit(line[i]) && spell == -1) {
        i--;
        spell = isSpelledNum(line, i);
    }
    number = number * 10 + (spell == -1 ? line[i] - '0' : spell);

    return number;
}

std::pair<int, int> day01()
{
    auto input = readInput("inputs/day01.txt");

    int sum1 = 0;
    for (auto i : input) {
        sum1 += process1(i);
    }

    int sum2 = 0;
    for (auto i : input) {
        sum2 += process2(i);
    }

    return {sum1, sum2};
}