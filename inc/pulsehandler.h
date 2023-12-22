#ifndef PULSEHANDLER_H
#define PULSEHANDLER_H

#include <string>
#include <queue>
#include <map>

#include "modules.h"

#include <iostream>

class PulseHandler
{
public:
    PulseHandler() = default;

    void addModule(Module* module) { modules[module->name] = module; }

    bool doCycle(bool quitEarly);
    void sendPulse(Module* src, std::string dst, bool pulse);

    void initModules();

    std::pair<int, int> getCounts() { return {lowCount, highCount}; }

    static int cyclesCount;

private:
    int lowCount = 0;
    int highCount = 0;
    std::queue<std::tuple<Module*, Module*, bool>> pulseQueue;
    std::map<std::string, Module*> modules;
};

#endif