#ifndef MODULES_H
#define MODULES_H

#include <string>
#include <vector>
#include <map>

class PulseHandler;

class Module
{
public:
    friend class PulseHandler;

    Module() = default;
    Module(std::string name, PulseHandler* handler) : name(name), handler(handler) { }
    Module(std::string name, PulseHandler* handler, std::vector<std::string>& destinations) : name(name), handler(handler), destinations(destinations) { }

    virtual void receivePulse(Module* src, bool pulse) { };

    std::string name;
    
protected:
    PulseHandler* handler;
    std::vector<std::string> destinations;
};

class FlipFlopModule : public Module
{
public:
    FlipFlopModule() = default;
    FlipFlopModule(std::string name, std::vector<std::string>& destinations, PulseHandler* handler) : Module(name, handler, destinations) { }

    void receivePulse(Module* src, bool pulse);
    
private:
    bool state;
};

class ConjunctionModule : public Module
{
public:
    ConjunctionModule() = default;
    ConjunctionModule(std::string name, std::vector<std::string>& destinations, PulseHandler* handler) : Module(name, handler, destinations) { }

    void receivePulse(Module* src, bool pulse);
    void addSource(std::string source) {
        sources[source] = false;
    }
    
private:
    std::map<std::string, bool> sources;
};

class BroadcastModule : public Module
{
public:
    BroadcastModule() = default;
    BroadcastModule(std::string name, std::vector<std::string>& destinations, PulseHandler* handler) : Module(name, handler, destinations) { }

    void receivePulse(Module* src, bool pulse);
};

class ButtonModule : public Module
{
public:
    ButtonModule() = default;
    ButtonModule(std::string name, PulseHandler* handler) : Module(name, handler) { }

    void receivePulse(Module* src, bool pulse);
};


#endif