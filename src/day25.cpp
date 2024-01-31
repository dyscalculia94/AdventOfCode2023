#include "day25.h"

#include <iostream>

#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "util.h"

namespace
{
    std::map<std::string, int> namesMap;
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
    
    using Input = std::vector<std::vector<int>>;

    int getVertex(Input& input, std::map<std::string, int>& namesMap, std::string vertexName)
    {
        auto vertexIt = namesMap.find(vertexName);
        int vertex = -1;
        if (vertexIt != namesMap.end()) {
            vertex = vertexIt->second;
        }
        else {
            vertex = input.size();
            input.push_back({});
            namesMap[vertexName] = vertex;
        }

        return vertex;
    }

    void addEdge(Input& input, int v, int u)
    {
        input[v].push_back(u);
        input[u].push_back(v);
    }

    void removeEdge(Input& input, int v, int u)
    {
        input[v].erase(std::find(input[v].begin(), input[v].end(), u));
        input[u].erase(std::find(input[u].begin(), input[u].end(), v));
    }

    Input processInput(const std::vector<std::string>& input)
    {
        Input res;

        for (auto line : input) {
            auto splitLine = split(line, ": ");

            int v = getVertex(res, namesMap, splitLine[0]);
            auto others = split(splitLine[1], " ");
            for (auto other : others) {
                int u = getVertex(res, namesMap, other);
                addEdge(res, v, u);
            }
        }

        return res;
    }

    void dfs(const Input& input, int vertex, std::vector<bool>& visited, int& count)
    {
        if (visited[vertex]) {
            return;
        }

        visited[vertex] = true;
        count++;

        for (auto v : input[vertex]) {
            dfs(input, v, visited, count);
        }
    }

    int countNeighbours(const Input& input, int vertex)
    {
        std::vector<bool> visited(input.size());

        int res = 0;
        dfs(input, vertex, visited, res);

        return res;
    }
    
    long long process1(Input& input)
    {
        long long res = 0;

        // I solved this one by converting the graph to SVG file and checking which wires I had to cut.
        // I've decided to do this after spending a few hours trying to get different solutions to work
        // (both mine and other people's). I'm just too exhausted to try anymore. Q.Q

        removeEdge(input, namesMap["sqh"], namesMap["jbz"]);
        removeEdge(input, namesMap["nvg"], namesMap["vfj"]);
        removeEdge(input, namesMap["fch"], namesMap["fvh"]);

        int count = countNeighbours(input, 0);

        res = count * (input.size() - count);

        return res;
    }

    long long process2(const Input& input)
    {
        long long res = -1;

        // The last day only has 1 part. Hurrah!

        return res;
    }
}

std::pair<long long, long long> day25()
{
    auto rawInput = readInput("inputs/day25.txt");
    auto input = processInput(rawInput);
    return {process1(input), process2(input)};
}