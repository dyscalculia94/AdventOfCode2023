#include "day20.h"

#include <iostream>

#include <fstream>
#include <string>
#include <vector>
#include <map>

#include <numeric>

#include "util.h"
#include "pulsehandler.h"

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

    using Input = PulseHandler*;

    Input processInput(const std::vector<std::string>& input)
    {
        Input res = new PulseHandler();

        for (auto line : input) {
            auto splits = split(line, " -> ");

            std::string name = splits[0];
            char type = '-';
            if (splits[0][0] == '%' || splits[0][0] == '&') {
                type = name[0];
                name = name.substr(1);
            }

            auto destinations = split(splits[1], ", ");

            Module* module = nullptr;
            switch (type) {
                case '-':
                    module = new BroadcastModule(name, destinations, res);
                    break;
                case '%':
                    module = new FlipFlopModule(name, destinations, res);
                    break;
                case '&':
                    module = new ConjunctionModule(name, destinations, res);
                    break;
            }
            res->addModule(module);
        }

        res->addModule(new ButtonModule("button", res));

        res->initModules();

        return res;
    }

    long long process1(const Input& input)
    {
        long long res = 0;

        const int cycles = 1000;
        for (int i = 0; i < cycles; i++) {
            input->doCycle(false);
        }
        auto counts = input->getCounts();
        res = counts.first * counts.second;

        return res;
    }

    long long process2(const Input& input)
    {
        long long res = 0;

        input->addModule(new Module("rx", input));

        // By analysing the input, we determine that, in order to know how many cycles it takes for module "rx" to receive low pulse,
        // we need to know lowest common multiple (LCM) of the cycle numbers when the inputs to the input of "rx" are sending high pulses.
        // By analysing the cycles, we determine that the numbers are following:
        const long long trCounts = 3739;
        const long long xmCounts = 3761;
        const long long drCounts = 3797;
        const long long nhCounts = 3889;

        res = std::lcm(std::lcm(std::lcm(trCounts, xmCounts), drCounts), nhCounts);

        return res;
    }
}

std::pair<long long, long long> day20()
{
    auto rawInput = readInput("inputs/day20.txt");
    auto input = processInput(rawInput);
    return {process1(input), process2(input)};
}