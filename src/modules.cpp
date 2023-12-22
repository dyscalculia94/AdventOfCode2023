#include "modules.h"
#include "pulsehandler.h"


void FlipFlopModule::receivePulse(Module* src, bool pulse)
{
    if (pulse) {
        return;
    }

    state = !state;
    for (auto dst : destinations) {
        handler->sendPulse(this, dst, state);
    }
}

void ConjunctionModule::receivePulse(Module* src, bool pulse)
{
    sources[src->name] = pulse;

    bool output = false;
    std::vector<std::pair<std::string, bool>> srcs(sources.begin(), sources.end());
    for (auto source : sources) {
        if (!source.second) {
            output = true;
        }
    }

    for (auto dst : destinations) {
        handler->sendPulse(this, dst, output);
    }
}

void BroadcastModule::receivePulse(Module* src, bool pulse)
{
    for (auto dst : destinations) {
        handler->sendPulse(this, dst, pulse);
    }
}

void ButtonModule::receivePulse(Module* src, bool pulse)
{
    handler->sendPulse(this, "broadcaster", false);
}
