#include "pulsehandler.h"
#include "modules.h"

#include <iostream>

int PulseHandler::cyclesCount = 0;

bool PulseHandler::doCycle(bool quitEarly)
{
    cyclesCount++;
    modules["button"]->receivePulse(nullptr, false);

    while(!pulseQueue.empty()) {
        auto [src, dst, pulse] = pulseQueue.front(); pulseQueue.pop();
        pulse ? highCount++ : lowCount++;

        if (dst) {
            dst->receivePulse(src, pulse);
        }
    }

    return true;
}

void PulseHandler::sendPulse(Module* src, std::string dst, bool pulse)
{
    pulseQueue.push({src, modules[dst], pulse});
}

void PulseHandler::initModules()
{
    for (auto module : modules) {
        if (!module.second) {
            continue;
        }

        for (auto dest : module.second->destinations) {
            if (auto conjunction = dynamic_cast<ConjunctionModule*>(modules[dest])) {
                conjunction->addSource(module.first);
            }
        }
    }
}
