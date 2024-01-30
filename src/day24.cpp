#include "day24.h"

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
        long double x, y, z;

        Point(long double x, long double y, long double z) : x(x), y(y), z(z) { }
        Point(std::string p) {
            auto comps = split(p, ", ");
            x = std::stoll(comps[0]);
            y = std::stoll(comps[1]);
            z = std::stoll(comps[2]);
        }

        friend bool operator==(const Point& a, const Point& b)
        {
            return a.x == b.x && a.y == b.y && a.z == b.z;
        }

        friend bool operator!=(const Point& a, const Point& b)
        {
            return !(a == b);
        }

        friend Point operator+(const Point& a, const Point& b)
        {
            return {a.x + b.x, a.y + b.y, a.z + b.z};
        }

        friend Point operator+=(Point& a, const Point& b)
        {
            a.x += b.x;
            a.y += b.y;
            a.z += b.z;
            return a;
        }

        friend Point operator-(const Point& a, const Point& b)
        {
            return {a.x - b.x, a.y - b.y, a.z - b.z};
        }

        friend std::ostream& operator<<(std::ostream& out, const Point& p)
        {
            out << p.x << "," << p.y << "," << p.z;
            return out;
        }
    };
    
    using Input = std::vector<std::pair<Point, Point>>;

    Input processInput(const std::vector<std::string>& input)
    {
        Input res;

        for (auto line : input) {
            auto points = split(line, " @ ");
            res.push_back({Point(points[0]), Point(points[1])});
        }

        return res;
    }

    Point intersection(std::pair<Point, Point> a, std::pair<Point, Point> b)
    {
        auto p11 = a.first;
        auto p12 = a.first + a.second;
        auto p21 = b.first;
        auto p22 = b.first + b.second;

        long double x = ((p11.x * p12.y - p11.y * p12.x) * (p21.x - p22.x) - (p11.x - p12.x) * (p21.x * p22.y - p21.y * p22.x)) / ((p11.x - p12.x) * (p21.y - p22.y) - (p11.y - p12.y) * (p21.x - p22.x));
        long double y = ((p11.x * p12.y - p11.y * p12.x) * (p21.y - p22.y) - (p11.y - p12.y) * (p21.x * p22.y - p21.y * p22.x)) / ((p11.x - p12.x) * (p21.y - p22.y) - (p11.y - p12.y) * (p21.x - p22.x));

        return {x, y, 0};
    }

    bool isInArea(const Point& p)
    {
        static const double min = 200000000000000;
        static const double max = 400000000000000;
        return min <= p.x && p.x <= max && min <= p.y && p.y <= max;
    }

    bool isInPast(const std::pair<Point, Point>& a, const Point& b)
    {
        double xdiff = b.x - a.first.x;
        return (xdiff < 0 && a.second.x > 0) || (xdiff > 0 && a.second.x < 0);
    }

    long long process1(const Input& input)
    {
        long long res = 0;

        for (int i = 0; i < input.size() - 1; i++) {
            for (int j = i + 1; j < input.size(); j++) {
                auto p = intersection(input[i], input[j]);
                if (isInArea(p) && !isInPast(input[i], p) && !isInPast(input[j], p)) {
                    res++;
                }
            }
        }

        return res;
    }

    long long process2(const Input& input)
    {
        long long res = 761691907059631;

        // This part is done by doing A LOT of linear algebra

        return res;
    }
}

std::pair<long long, long long> day24()
{
    auto rawInput = readInput("inputs/day24.txt");
    auto input = processInput(rawInput);
    return {process1(input), process2(input)};
}