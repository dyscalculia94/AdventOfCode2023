#include "day23.h"

#include <iostream>

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <queue>

#include <algorithm>

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

    struct Point
    {
        int x, y;

        double mod() const
        {
            return std::sqrt((100 * x) * (100 * x) + y * y);
        }

        friend bool operator==(const Point& a, const Point& b)
        {
            return a.x == b.x && a.y == b.y;
        }

        friend bool operator!=(const Point& a, const Point& b)
        {
            return !(a == b);
        }

        friend bool operator<(const Point& a, const Point& b)
        {
            return a.mod() < b.mod();
        }

        friend Point operator+(const Point& a, const Point& b)
        {
            return {a.x + b.x, a.y + b.y};
        }

        friend Point operator+=(Point& a, const Point& b)
        {
            a.x += b.x;
            a.y += b.y;
            return a;
        }

        friend Point operator-(const Point& a, const Point& b)
        {
            return {a.x - b.x, a.y - b.y};
        }

        friend std::ostream& operator<<(std::ostream& out, const Point& p)
        {
            out << p.x << "," << p.y;
            return out;
        }
    };

    Point up = {-1, 0};
    Point right = {0, 1};
    Point down = {1, 0};
    Point left = {0, -1};
    Point dirs[] = {up, right, down, left};
    
    using Input = std::vector<std::string>;

    Input processInput(const std::vector<std::string>& input)
    {
        return input;
    }

    using Graph = std::map<Point, std::vector<std::pair<Point, int>>>;

    bool isValid(const Input& input, Point p)
    {
        return p.x >= 0 && p.x < input.size() && p.y >= 0 && p.y < input.front().size();
    }

    bool isWall(const Input& input, Point p)
    {
        return input[p.x][p.y] == '#';
    }

    bool isPath(const Input& input, Point p)
    {
        return isValid(input, p) && !isWall(input, p);
    }

    bool isValidSlope(const Input& input, Point p, Point dir)
    {
        Point next = p + dir;
        return (dir == up && input[next.x][next.y] == '^') ||
                (dir == right && input[next.x][next.y] == '>') ||
                (dir == down && input[next.x][next.y] == 'v') ||
                (dir == left && input[next.x][next.y] == '<');
    }

    bool isIntersection(const Input& input, Point p)
    {
        static Point end = {static_cast<int>(input.size() - 1), static_cast<int>(input.back().size() - 2)};
        return (p == end) || isPath(input, p + up) + isPath(input, p + right) + isPath(input, p + down) + isPath(input, p + left) > 2;
    }

    Point getOppositeDir(Point dir)
    {
        return {dir.x * -1, dir.y * -1};
    }

    Point getNextDir(const Input& input, Point current, Point prev)
    {
        Point prevDir = prev - current;

        for (auto dir : dirs) {
            if (dir != prevDir && isPath(input, current + dir)) {
                return dir;
            }
        }

        return up;
    }

    std::tuple<Point, Point, int> getNextIntersection(const Input& input, Point current, Point dir)
    {
        Point prev = current;
        current += dir;
        int steps = 1;
        while (!isIntersection(input, current)) {
            dir = getNextDir(input, current, prev);
            prev = current;
            current += dir;
            steps++;
        }

        return {current, getOppositeDir(dir), steps};
    }

    void buildGraph(const Input& input, Graph& graph, bool isSlippery)
    {
        Point start = {0, 1};

        // First point in the pair is a valid location within the maze,
        // the second point is the direction in which we should move
        std::queue<std::pair<Point, Point>> q;
        q.push({start, down});
        std::set<std::pair<Point, Point>> seen;
        while(!q.empty()) {
            auto [current, dir] = q.front(); q.pop();
            if (seen.find({current, dir}) != seen.end()) {
                continue;
            }
            seen.insert({current, dir});

            auto [next, arrivalDir, steps] = getNextIntersection(input, current, dir);
            for (auto dir : dirs) {
                if (dir != arrivalDir && isPath(input, next + dir) && (!isSlippery || isValidSlope(input, next, dir))) {
                    q.push({next, dir});
                }
            }

            auto& neighbours = graph[current];
            if (std::find(neighbours.begin(), neighbours.end(), std::make_pair(next, steps)) == neighbours.end()) {
                neighbours.push_back({next, steps});
                if (!isSlippery) {
                    graph[next].push_back({current, steps});
                }
            }
        }
    }

    int dfs(const Input& input, Graph& graph, std::set<Point>& seen, Point current, int steps)
    {
        static Point end = {static_cast<int>(input.size() - 1), static_cast<int>(input.back().size() - 2)};
        if (current == end) {
            return steps;
        }

        if (seen.find(current) != seen.end()) {
            return 0;
        }
        seen.insert(current);

        int res = 0;
        for (auto next : graph[current]) {
            res = std::max(res, dfs(input, graph, seen, next.first, steps + next.second));
        }

        seen.erase(current);

        return res;
    }

    long long process(const Input& input, bool isSlippery)
    {
        long long res = 0;

        Graph graph;
        buildGraph(input, graph, isSlippery);

        std::set<Point> seen;
        res = dfs(input, graph, seen, {0, 1}, 0);

        return res;
    }

    long long process1(const Input& input)
    {
        return process(input, true);
    }

    long long process2(const Input& input)
    {
        return process(input, false);
    }
}

std::pair<long long, long long> day23()
{
    auto rawInput = readInput("inputs/day23.txt");
    auto input = processInput(rawInput);
    return {process1(input), process2(input)};
}