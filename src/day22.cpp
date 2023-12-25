#include "day22.h"

#include <iostream>

#include <fstream>
#include <string>
#include <vector>
#include <set>

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
        int x, y, z;

        void parse(std::string str)
        {
            auto coords = split(str, ",");
            x = std::stoi(coords[0]);
            y = std::stoi(coords[1]);
            z = std::stoi(coords[2]);
        }

        friend bool operator==(const Point& a, const Point& b)
        {
            return a.x == b.x && a.y == b.y && a.z == b.z;
        }

        friend bool operator!=(const Point& a, const Point& b)
        {
            return !(a == b);
        }

        friend bool operator<(const Point& a, const Point& b)
        {
            if (a.z != b.z) {
                return a.z < b.z;
            }

            if (a.x != b.x) {
                return a.x < b.x;
            }

            return a.y < b.y;
        }

        friend std::ostream& operator<<(std::ostream& out, const Point& p)
        {
            out << p.x << "," << p.y << "," << p.z;
            return out;
        }
    };

    struct Brick
    {
        Point start, end;
        int supports = 0;

        Brick(std::string str) { parse(str); }

        void parse(std::string str)
        {
            auto points = split(str, "~");
            start.parse(points[0]);
            end.parse(points[1]);
        }

        bool justAbove(const Brick& b)
        {
            return start.z == b.end.z + 1;
        }

        bool isOnTopOf(const Brick& b)
        {
            return justAbove(b)
                && ((start.x <= b.start.x && b.start.x <= end.x) || (b.start.x <= start.x && start.x <= b.end.x))
                && ((start.y <= b.start.y && b.start.y <= end.y) || (b.start.y <= start.y && start.y <= b.end.y));
        }

        bool isOnGround()
        {
            return start.z == 1;
        }

        friend bool operator==(const Brick& a, const Brick& b)
        {
            return a.start == b.start && a.end == b.end;
        }

        friend bool operator!=(const Brick& a, const Brick& b)
        {
            return !(a == b);
        }

        friend bool operator<(const Brick& a, const Brick& b)
        {
            if (a.start != b.start) {
                return a.start < b.start;
            }

            return a.end < b.end;
        }

        friend std::ostream& operator<<(std::ostream& out, const Brick& b)
        {
            out << b.start << "~" << b.end;
            return out;
        }
    };

    using Input = std::vector<Brick>;

    Input processInput(const std::vector<std::string>& input)
    {
        Input res;

        for (auto line : input) {
            res.push_back({line});
        }

        return res;
    }

    void fall(Input& input)
    {
        for (int i = 0; i < input.size(); i++) {
            if (input[i].start.z == 1) {
                continue;
            }

            // Move the brick down until we hit another brick
            bool falling = true;
            while (falling) {
                if (input[i].start.z == 1) {
                    break;
                }
                for (int j = 0; j < i; j++) {

                    if (input[i].isOnTopOf(input[j])) {
                        falling = false;
                        break;
                    }
                }

                if (falling) {
                    input[i].start.z--;
                    input[i].end.z--;
                }
            }
        }
    }

    long long process1(Input input)
    {
        long long res = 0;

        std::sort(input.begin(), input.end());
        
        fall(input);

        std::set<Brick> critical;
        for (int i = 0; i < input.size(); i++) {
            if (input[i].isOnGround()) {
                continue;
            }

            std::set<Brick> supportedBy;
            for (int j = 0; j < i; j++) {
                if (input[i].isOnTopOf(input[j])) {
                    supportedBy.insert(input[j]);
                }
            }
            if (supportedBy.size() == 1) {
                critical.insert(supportedBy.begin(), supportedBy.end());
            }
        }

        return input.size() - critical.size();
    }

    long long process2(Input input)
    {
        long long res = 0;

        std::sort(input.begin(), input.end());
        
        fall(input);

        for (int i = 0; i < input.size(); i++) {
            Input backup = input;

            input[i].start.x = 1000;
            input[i].end.x = 1000;
            fall(input);

            int count = 0;
            for (int j = 0; j < input.size(); j++) {
                if (j != i && memcmp(&input[j], &backup[j], sizeof(Brick)) != 0) {
                    count++;
                }
            }
            res += count;

            input = backup;
	    }

        return res;
    }
}

std::pair<long long, long long> day22()
{
    auto rawInput = readInput("inputs/day22.txt");
    auto input = processInput(rawInput);
    return {process1(input), process2(input)};
}