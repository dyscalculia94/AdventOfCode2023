#include "day16.h"

#include <iostream>

#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_set>

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

    enum class Direction
    {
        up,
        right,
        down,
        left
    };

    std::string getDirectionName(Direction dir)
    {
        switch (dir) {
            case Direction::up: return "up";
            case Direction::right: return "right";
            case Direction::down: return "down";
            case Direction::left: return "left";
        }

        return "";
    }

    struct Photon
    {
        int i;
        int j;
        Direction dir;

        friend bool operator==(const Photon& a, const Photon& b) { return a.i == b.i && a.j == b.j && a.dir == b.dir; }
        std::string getId() const { return std::to_string(i) + "," + std::to_string(j) + "," + getDirectionName(dir); }
    };

    bool isValid(const Input& input, const Photon& p)
    {
        return p.i >= 0 && p.i < input.size() && p.j >= 0 && p.j < input[p.i].size();
    }

    Photon getNext(const Photon& p, Direction dir)
    {
        int i = 0, j = 0;

        switch (dir) {
            case Direction::up:
                i = -1;
                j = 0;
                break;
            case Direction::right:
                i = 0;
                j = 1;
                break;
            case Direction::down:
                i = 1;
                j = 0;
                break;
            case Direction::left:
                i = 0;
                j = -1;
                break;
        }

        return {p.i + i, p.j + j, dir};
    }

    Direction getMirror(char mirror, Direction dir)
    {
        switch (dir) {
            case Direction::up:
                return mirror == '/' ? Direction::right : Direction::left;
            case Direction::right:
                return mirror == '/' ? Direction::up : Direction::down;
            case Direction::down:
                return mirror == '/' ? Direction::left : Direction::right;
            case Direction::left:
                return mirror == '/' ? Direction::down : Direction::up;
        }

        return Direction::right;
    }

    void print(std::vector<std::vector<bool>> visited)
    {
        for (auto i : visited) {
            for (auto j : i) {
                std::cout << (j ? "#" : ".");
            }
            std::cout << std::endl;
        }
    }
    
    int process(const Input& input, Photon start)
    {
        int res = 0;

        std::vector<std::vector<bool>> visited(input.size(), std::vector<bool>(input.front().size(), false));

        auto hash = [](const Photon& p){ return std::hash<std::string>()(p.getId()); };
        
        std::stack<Photon> st;
        std::unordered_set<Photon, decltype(hash)> seen(0, hash);
        st.push(start);
        while (!st.empty()) {
            auto current = st.top(); st.pop();
            if (!isValid(input, current) || seen.count(current)) {
                continue;
            }

            seen.insert(current);
            visited[current.i][current.j] = true;

            switch (input[current.i][current.j]) {
                case '|':
                    if (current.dir == Direction::right || current.dir == Direction::left) {
                        st.push(getNext(current, Direction::up));
                        st.push(getNext(current, Direction::down));
                    }
                    else {
                        st.push(getNext(current, current.dir));
                    }
                    break;
                case '-':
                    if (current.dir == Direction::up || current.dir == Direction::down) {
                        st.push(getNext(current, Direction::right));
                        st.push(getNext(current, Direction::left));
                    }
                    else {
                        st.push(getNext(current, current.dir));
                    }
                    break;
                case '/':
                case '\\':
                    st.push(getNext(current, getMirror(input[current.i][current.j], current.dir)));
                    break;
                default:
                    st.push(getNext(current, current.dir));
                    break;
            }
        }

        // print(visited);

        for (auto v : visited) {
            res += std::count(v.begin(), v.end(), true);
        }

        return res;
    }

    int process1(const Input& input)
    {
        return process(input, {0, 0, Direction::right});
    }

    int process2(const Input& input)
    {
        int res = 0;

        for (int i = 0; i < input.size(); i++) {
            res = std::max(res, process(input, {i, 0, Direction::right}));
            res = std::max(res, process(input, {i, static_cast<int>(input[i].size() - 1), Direction::left}));
        }

        for (int i = 0; i < input.front().size(); i++) {
            res = std::max(res, process(input, {0, i, Direction::down}));
            res = std::max(res, process(input, {static_cast<int>(input.size() - 1), i, Direction::up}));
        }

        return res;
    }
}

std::pair<int, int> day16()
{
    auto rawInput = readInput("inputs/day16.txt");
    auto input = processInput(rawInput);
    return {process1(input), process2(input)};
}