#include "day10.h"

#include <iostream>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <map>

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

    std::pair<int, int> getStart(const Input& input)
    {
        for (int i = 0; i < input.size(); i++) {
            for (int j = 0; j < input[i].size(); j++) {
                if (input[i][j] == 'S') {
                    return {i, j};
                }
            }
        }

        return {-1, -1};
    }

    enum class Direction
    {
        up,
        right,
        down,
        left,
        num
    };

    Direction getOpposite(Direction dir) {
        switch(dir) {
            case Direction::up: return Direction::down;
            case Direction::right: return Direction::left;
            case Direction::down: return Direction::up;
            case Direction::left: return Direction::right;
            default: return Direction::num;
        }
    }

    Direction findNext(const Input& input, int i, int j, Direction prev)
    {
        switch(input[i][j]) {
            case '|':
            case '-':
                return getOpposite(prev);
            case 'L':
                return prev == Direction::right ? Direction::up : Direction::right;
            case 'J':
                return prev == Direction::left ? Direction::up : Direction::left;
            case '7':
                return prev == Direction::left ? Direction::down : Direction::left;
            case 'F':
                return prev == Direction::right ? Direction::down : Direction::right;
        }

        return Direction::up;
    }

    std::pair<int, int> getNext(const Input& input, std::pair<int, int> current, Direction next) {
        switch(next) {
            case Direction::up: return {current.first - 1, current.second};
            case Direction::right: return {current.first, current.second + 1};
            case Direction::down: return {current.first + 1, current.second};
            case Direction::left: return {current.first, current.second - 1};
            default: return {-1, -1};
        }
    }

    bool isProperConnection(const Input& input, std::pair<int, int> current, Direction nextDirection)
    {
        auto next = getNext(input, current, nextDirection);

        char nextPipe = input[next.first][next.second];
        switch(nextDirection) {
            case Direction::up: return nextPipe == '|' || nextPipe == '7' || nextPipe == 'F';
            case Direction::right: return nextPipe == '-' || nextPipe == 'J' || nextPipe == '7';
            case Direction::down: return nextPipe == '|' || nextPipe == 'L' || nextPipe == 'J';
            case Direction::left: return nextPipe == '-' || nextPipe == 'L' || nextPipe == 'F';
            default: return false;
        }
    }

    struct Position
    {
        std::pair<int, int> pos;
        int dist;
        Direction prev;
    };

    std::vector<Direction> getInitialDirections(const Input& input, std::pair<int, int> current)
    {
        std::vector<Direction> initial;

        if (isProperConnection(input, current, Direction::up)) {
            initial.push_back(Direction::up);
        }

        if (isProperConnection(input, current, Direction::right)) {
            initial.push_back(Direction::right);
        }

        if (isProperConnection(input, current, Direction::down)) {
            initial.push_back(Direction::down);
        }

        if (isProperConnection(input, current, Direction::left)) {
            initial.push_back(Direction::left);
        }

        return initial;
    }

    void traverse(const Input& input, std::vector<std::vector<int>>& dist, std::pair<int, int> start, Direction dir) {
        std::pair<int, int> next = getNext(input, start, dir);
        Direction prev = getOpposite(dir);
        int count = 1;
        while (next != start) {
            if (dist[next.first][next.second] == -1) {
                dist[next.first][next.second] = count;
            }
            else {
                dist[next.first][next.second] = std::min(count, dist[next.first][next.second]);
            }
            count++;

            dir = findNext(input, next.first, next.second, prev);
            prev = getOpposite(dir);
            next = getNext(input, next, dir);
        }
    }

    int process1(const Input& input)
    {
        int res = 0;

        auto start = getStart(input);
        std::vector<std::vector<int>> dist(input.size(), std::vector<int>(input[0].size(), -1));
        dist[start.first][start.second] = 0;

        // Find the directions for the main loop to go
        auto initial = getInitialDirections(input, start);
        for (auto i : initial) {
            traverse(input, dist, start, i);
        }

        for (int i = 0; i < dist.size(); i++) {
            for (int j = 0; j < dist[i].size(); j++) {
                if (dist[i][j] != -1 && dist[i][j] > res) {
                    res = dist[i][j];
                }
            }
        }

        return res;
    }

    void findPipeline(Input& input, std::vector<std::vector<int>>& dist, std::pair<int, int> start, Direction dir) {
        std::pair<int, int> next = getNext(input, start, dir);
        input[start.first][start.second] = '*';
        Direction prev = getOpposite(dir);
        while (next != start) {
            dir = findNext(input, next.first, next.second, prev);
            prev = getOpposite(dir);
            input[next.first][next.second] = '*';
            next = getNext(input, next, dir);
        }
    }

    int process2(Input& input)
    {
        int res = 0;

        auto start = getStart(input);
        std::vector<std::vector<int>> dist(input.size(), std::vector<int>(input[0].size(), -1));
        dist[start.first][start.second] = 0;

        Input map = input;
        // Find the directions for the main loop to go
        auto initial = getInitialDirections(map, start);
        findPipeline(map, dist, start, initial.front());

        for (int i = 0; i < map.size(); i++) {
            for (int j = 0; j < map[i].size(); j++) {
                if (map[i][j] != '*') {
                    input[i][j] = '.';
                }
            }
        }

        std::map<std::pair<Direction, Direction>, char> startPipes = {
            {{Direction::up, Direction::down}, '|'},
            {{Direction::up, Direction::right}, 'L'},
            {{Direction::up, Direction::left}, 'J'},
            {{Direction::down, Direction::left}, '7'},
            {{Direction::right, Direction::down}, 'F'},
            {{Direction::right, Direction::left}, '-'},
        };
        char startPipe = startPipes[{initial[0], initial[1]}];
        input[start.first][start.second] = startPipe;

        char other;
        for (int i = 0; i < input.size(); i++) {
            bool inside = false;
            std::stack<char> st;
            for (int j = 0; j < input[i].size(); j++) {
                switch (input[i][j]) {
                    case '|':
                        inside = !inside;
                        break;
                    case '-':
                        break;
                    case 'L':
                        st.push(input[i][j]);
                        break;
                    case 'J':
                        other = st.top(); st.pop();
                        if (other == 'F') {
                            inside = !inside;
                        }
                        break;
                    case '7':
                        other = st.top(); st.pop();
                        if (other == 'L') {
                            inside = !inside;
                        }
                        break;
                    case 'F':
                        st.push(input[i][j]);
                        break;
                    case '.':
                        if (inside) {
                            res++;
                        }
                }
            }
        }
        

        // for (int i = 0; i < dist.size(); i++) {
        //     for (int j = 0; j < dist[i].size(); j++) {
        //         if (dist[i][j] != -1) {
        //             break;
        //         }
        //     }
        //     for (int j = dist[i].size() - 1; j >= 0; j--) {
        //         if (dist[i][j] != -1) {
        //             break;
        //         }
        //     }
        // }

        // for (auto i : dist) {
        //     res += std::count(i.begin(), i.end(), -1);
        // }

        return res;
    }
}

std::pair<int, int> day10()
{
    auto input = readInput("inputs/day10.txt");
    return {process1(input), process2(input)};
}