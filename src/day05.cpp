#include "day05.h"

#include <iostream>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <limits>


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

    struct Range
    {
        friend bool operator<(const Range& a, const Range& b) { return a.start < b.start; }

        bool contains(const Range& r) const { return start <= r.start && end >= r.end; }
    
        bool overlaps(const Range& r) { return (start >= r.start && start <= r.end) || (end >= r.start && end <= r.end) || contains(r) || r.contains(*this); }

        void merge(const Range& r) { start = std::min(start, r.start); end = std::max(end, r.end); }

        friend std::ostream& operator<<(std::ostream& out, Range& r) { out << r.start << " " << r.end; return out; }

        long long start;
        long long end;
    };

    struct Map
    {
        bool isContained(long long val) { return val >= src && val <= src + len; }

        long long getOffset() const { return dest - src; }

        friend bool operator<(const Map& a, const Map& b) { return a.src < b.src; }

        friend std::ostream& operator<<(std::ostream& out, Map& r) { out << r.src << " " << r.src + r.len << " " << r.getOffset(); return out; }
        long long src;
        long long dest;
        long long len;
    };

    std::vector<Map> seed2soil;
    std::vector<Map> soil2fert;
    std::vector<Map> fert2water;
    std::vector<Map> water2light;
    std::vector<Map> light2temp;
    std::vector<Map> temp2hum;
    std::vector<Map> hum2loc;
    std::vector<std::vector<Map>*> maps = { &seed2soil, &soil2fert, &fert2water, &water2light, &light2temp, &temp2hum, &hum2loc};

    std::vector<long long> processInput(const std::vector<std::string>& input)
    {
        std::vector<long long> res;

        auto current = &seed2soil;
        for (auto line : input) {
            if (line.empty()) {
                continue;
            }

            if (line.find("seeds: ") != std::string::npos) {
                std::stringstream ss(line.substr(line.find(": ") + 2));
                
                long long seed;
                while (ss >> seed) {
                    res.push_back(seed);
                }

                continue;
            }

            if (line == "seed-to-soil map:") {
                current = &seed2soil;
                continue;
            }
            else if (line == "soil-to-fertilizer map:") {
                current = &soil2fert;
                continue;
            }
            else if (line == "fertilizer-to-water map:") {
                current = &fert2water;
                continue;
            }
            else if (line == "water-to-light map:") {
                current = &water2light;
                continue;
            }
            else if (line == "light-to-temperature map:") {
                current = &light2temp;
                continue;
            }
            else if (line == "temperature-to-humidity map:") {
                current = &temp2hum;
                continue;
            }
            else if (line == "humidity-to-location map:") {
                current = &hum2loc;
                continue;
            }

            std::stringstream ss(line);
            long long src, dest, len;
            ss >> dest >> src >> len;
            current->push_back({src, dest, len});
        }

        return res;
    }

    std::vector<Range> tidy(const std::vector<Range>& ranges)
    {
        std::vector<Range> res;

        Range current = ranges[0];
        for (int i = 1; i < ranges.size(); i++) {
            if (!current.overlaps(ranges[i])) {
                res.push_back(current);
                current = ranges[i];
            }
            else {
                current.merge(ranges[i]);
            }
        }
        res.push_back(current);

        return res;
    }   

    long long process1(const std::vector<long long>& input)
    {
        long long res = std::numeric_limits<long long>::max();

        for (auto i : input) {
            for (auto map : maps) {
                for (auto m : *map) {
                    if (m.isContained(i)) {
                        i += m.getOffset();
                        break;
                    }
                }
            }

            res = std::min(i, res);
        }

        return res;
    }

    long long process2(const std::vector<long long>& input)
    {
        long long res = std::numeric_limits<long long>::max();

        for (int j = 0; j < input.size(); j += 2) {

            for (long long i = input[j]; i < input[j] + input[j + 1]; i++) {
                long long temp = i;
                for (auto map : maps) {
                    for (auto m : *map) {
                        if (m.isContained(temp)) {
                            temp += m.getOffset();
                            break;
                        }
                    }
                }

                res = std::min(temp, res);
            }
        }

        return res;
    }
}

std::pair<int, int> day05()
{
    auto rawInput = readInput("inputs/day05.txt");
    auto input = processInput(rawInput);
    return {process1(input), process2(input)};
}