#include "day17.h"

#include <iostream>

#include <fstream>
#include <string>
#include <vector>
#include <queue>
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

    Input processInput(const std::vector<std::string>& input)
    {
        return input;
    }

    enum class Direction
    {
        straight,
        right,
        left
    };

    struct Node
    {
        Node(int i, int j, Direction dir, Node* prev, int dist=0) : i(i), j(j), dir(dir), prev(prev), dist(dist) { }
        int i;
        int j;
        Direction dir;
        Node* prev;
        int dist;

        int straightLength() const
        {
            int count = 0;
            Node* current = const_cast<Node*>(this);
            while (current && current->dir == Direction::straight) {
                current = current->prev;
                count++;
            }

            return count;
        }

        friend bool comparePos(const Node* a, const Node* b)
        {
            if ((a && !b) || (b && !a)) {
                return false;
            }
            return a->i == b->i && a->j == b->j;
        }

        friend bool operator==(const Node& a, const Node& b)
        {
            return comparePos(&a, &b) && comparePos(a.prev, b.prev) && a.straightLength() == b.straightLength();
        }
    };

    bool isValid(const Input& input, Node* current)
    {
        return current->i >= 0 && current->i < input.size() && current->j >= 0 && current->j < input.front().size();
    }

    std::pair<int, int> getStraightDiff(Node* current)
    {
        return {current->i - current->prev->i, current->j - current->prev->j};
    }

    std::pair<int, int> getRightDiff(std::pair<int, int> diff)
    {
        if (diff.first) {
            return diff.first < 0 ? std::make_pair(0, 1) : std::make_pair(0, -1);
        }
        else {
            return diff.second < 0 ? std::make_pair(-1, 0) : std::make_pair(1, 0);
        }
    }

    std::pair<int, int> getLeftDiff(std::pair<int, int> diff)
    {
        if (diff.first) {
            return diff.first < 0 ? std::make_pair(0, -1) : std::make_pair(0, 1);
        }
        else {
            return diff.second < 0 ? std::make_pair(1, 0) : std::make_pair(-1, 0);
        }
    }

    Node* getStraight(Node* current)
    {
        auto [i, j] = getStraightDiff(current);
        return new Node(current->i + i, current->j + j, Direction::straight, current);
    }

    Node* getRight(Node* current)
    {
        auto [i, j] = getRightDiff(getStraightDiff(current));
        return new Node(current->i + i, current->j + j, Direction::right, current);
    }

    Node* getLeft(Node* current)
    {
        auto [i, j] = getLeftDiff(getStraightDiff(current));
        return new Node(current->i + i, current->j + j, Direction::left, current);
    }

    template<typename QueueType>
    void calculate(const Input& input, std::vector<std::vector<int>>& distance, QueueType& q, Node* next)
    {
        if (isValid(input, next)) {
            int dist = next->prev->dist + input[next->i][next->j] - '0';
            distance[next->i][next->j] = std::min(distance[next->i][next->j], dist);
            next->dist = dist;
            q.push(next);
        }
    }

    int process1(const Input& input)
    {
        int res = 0;

        std::vector<std::vector<int>> distance(input.size(), std::vector<int>(input.front().size(), std::numeric_limits<int>::max()));
        auto hash = [](const Node a) { return a.i + a.j; };
        std::unordered_set<Node, decltype(hash)> seen(0, hash);

        distance[0][0] = 0;
        Node start = {0, 0, Direction::left, nullptr};

        auto comp = [](const Node* a, const Node* b) { return a->dist > b->dist; };
        std::priority_queue<Node*, std::vector<Node*>, decltype(comp)> q(comp);
        q.push(&start);
        while (!q.empty()) {
            auto current = q.top(); q.pop();
            if (seen.find(*current) != seen.end()) {
                continue;
            }
            seen.insert(*current);

            if (current->i == input.size() - 1 && current->j == input.front().size() - 1) {
                res = current->dist;
                break;
            }

            if (current->i == 0 && current->j == 0) {
                calculate(input, distance, q, new Node(0, 1, Direction::straight, current));
                calculate(input, distance, q, new Node(1, 0, Direction::straight, current));
                continue;
            }

            calculate(input, distance, q, getLeft(current));
            calculate(input, distance, q, getRight(current));
            if (current->straightLength() < 2) {
                calculate(input, distance, q, getStraight(current));
            }
        }        

        return res;
    }

    int process2(const Input& input)
    {
        int res = 0;

        std::vector<std::vector<int>> distance(input.size(), std::vector<int>(input.front().size(), std::numeric_limits<int>::max()));
        auto hash = [](const Node a) { return a.i + a.j; };
        std::unordered_set<Node, decltype(hash)> seen(0, hash);

        distance[0][0] = 0;
        Node start = {0, 0, Direction::left, nullptr};

        auto comp = [](const Node* a, const Node* b) { return a->dist > b->dist; };
        std::priority_queue<Node*, std::vector<Node*>, decltype(comp)> q(comp);
        q.push(&start);
        while (!q.empty()) {
            auto current = q.top(); q.pop();
            if (seen.find(*current) != seen.end()) {
                continue;
            }
            seen.insert(*current);

            if (current->i == input.size() - 1 && current->j == input.front().size() - 1 && current->straightLength() >= 3) {
                res = current->dist;
                break;
            }

            if (current->i == 0 && current->j == 0) {
                calculate(input, distance, q, new Node(0, 1, Direction::straight, current));
                calculate(input, distance, q, new Node(1, 0, Direction::straight, current));
                continue;
            }

            if (current->straightLength() < 9) {
                calculate(input, distance, q, getStraight(current));
            }
            if (current->straightLength() > 2) {
                calculate(input, distance, q, getLeft(current));
                calculate(input, distance, q, getRight(current));
            }
        }        

        return res;
    }
}

std::pair<int, int> day17()
{
    auto rawInput = readInput("inputs/day17.txt");
    auto input = processInput(rawInput);
    return {process1(input), process2(input)};
}