#include "day19.h"

#include <iostream>

#include <fstream>
#include <string>
#include <vector>
#include <map>

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

    struct Part
    {
        int x;
        int m;
        int a;
        int s;

        int get(char category) {
            switch (category) {
                case 'x': return x;
                case 'm': return m;
                case 'a': return a;
                case 's': return s;
            }

            return -1;
        }

        int getVal() const {
            return x + m + a + s;
        }
    };

    enum Comparison
    {
        less,
        greater
    };

    struct Rule
    {
        char category;
        Comparison comp;
        int val;
        std::string next;

        bool isSatisfying(Part part) {
            return comp == Comparison::less ? part.get(category) < val : part.get(category) > val;
        }
    };

    struct Workflow
    {
        std::vector<Rule> rules;
    };

    Rule parseRule(const std::string& rule)
    {
        const bool isDefault = rule.find(':') == std::string::npos;
        if (isDefault) {
            return {'-', Comparison::less, 0, rule};
        }

        char category = rule[0];
        Comparison comp = rule[1] == '<' ? Comparison::less : Comparison::greater;
        auto valueAndNext = split(rule.substr(2), ":");
        return {category, comp, std::stoi(valueAndNext[0]), valueAndNext[1]};
    }

    int getValue(const std::string& str) {
        return std::stoi(str.substr(2));
    }

    Part parsePart(const std::string& part)
    {
        auto cats = split(part.substr(1, part.size() - 1), ",");
        return {getValue(cats[0]), getValue(cats[1]), getValue(cats[2]), getValue(cats[3])};
    }

    enum class RuleUpdateFlag
    {
        pass,
        fail
    };

    struct PartRange
    {
        std::pair<int, int> x;
        std::pair<int, int> m;
        std::pair<int, int> a;
        std::pair<int, int> s;

        PartRange()
        {
            x = std::make_pair(1, 4000);
            m = std::make_pair(1, 4000);
            a = std::make_pair(1, 4000);
            s = std::make_pair(1, 4000);
        }

        bool isValid(const std::pair<int, int>& range)
        {
            return range.first != 0;
        }

        bool isValid()
        {
            return isValid(x) && isValid(m) && isValid(a) && isValid(s);
        }

        std::pair<int, int>& get(char category) {
            switch (category) {
                case 'x': return x;
                case 'm': return m;
                case 'a': return a;
                case 's': return s;
            }

            return x;
        }

        void update(Rule rule, RuleUpdateFlag flag)
        {
            auto& cat = get(rule.category);

            if (flag == RuleUpdateFlag::pass) {
                if (rule.comp == Comparison::less) {
                    if (cat.first < rule.val) {
                        cat.second = std::min(cat.second, rule.val - 1);
                    }
                }
                else {
                    if (cat.second > rule.val) {
                        cat.first = std::max(cat.first, rule.val + 1);
                    }
                }
            }
            else {
                if (rule.comp == Comparison::less) {
                    if (cat.first < rule.val) {
                        cat.first = std::max(cat.first, rule.val);
                    }
                }
                else {
                    if (cat.second > rule.val) {
                        cat.second = std::min(cat.second, rule.val);
                    }
                }

                if (cat.first > cat.second) {
                    cat.first = cat.second = 0;
                }
            }
        }

        long long getCount(const std::pair<int, int>& range)
        {
            return range.second - range.first + 1;
        }
        
        long long getCount()
        {
            return getCount(x) * getCount(m) * getCount(a) * getCount(s);
        }
    };
    
    using Input = std::pair<std::map<std::string, Workflow>, std::vector<Part>>;

    Input processInput(const std::vector<std::string>& input)
    {
        std::map<std::string, Workflow> workflows;
        std::vector<Part> parts;
        int isWorkflow = true;
        for (auto line : input) {
            if (line.empty()) {
                isWorkflow = false;
                continue;
            }

            if (isWorkflow) {
                auto temp = split(line, "{");
                auto rulesStrings = split(temp[1].substr(0, temp[1].size() - 1), ",");
                std::vector<Rule> rules;
                for (auto rule : rulesStrings) {
                    rules.push_back(parseRule(rule));
                }
                workflows[temp[0]] = {rules};
            }
            else {
                parts.push_back(parsePart(line));
            }
        }

        return {workflows, parts};
    }

    long long process1(const Input& input)
    {
        long long res = 0;

        auto workflows = input.first;
        for (auto part : input.second) {
            std::string current = "in";
            while (current != "A" && current != "R") {
                auto rules = workflows[current].rules;
                for (auto rule : rules) {
                    if (rule.isSatisfying(part)) {
                        current = rule.next;
                        break;
                    }
                }
            }

            if (current == "A") {
                res += part.getVal();
            }
        }

        return res;
    }
    
    void traverse(std::map<std::string, Workflow>& workflows, std::vector<PartRange>& ranges, std::string current, PartRange range)
    {
        if (!range.isValid()) {
            return;
        }

        if (current == "A") {
            ranges.push_back(range);
            return;
        }

        if (current == "B") {
            return;
        }

        auto rules = workflows[current].rules;
        for (auto rule : rules) {
             if (rule.category == '-') {
                traverse(workflows, ranges, rule.next, range);
             }
             else {
                auto cat = range.get(rule.category);
                if (rule.comp == Comparison::less) {
                    if (cat.first < rule.val) {
                        auto newRange = range;
                        newRange.update(rule, RuleUpdateFlag::pass);
                        traverse(workflows, ranges, rule.next, newRange);
                    }
                }
                else {
                    if (cat.second > rule.val) {
                        auto newRange = range;
                        newRange.update(rule, RuleUpdateFlag::pass);
                        traverse(workflows, ranges, rule.next, newRange);
                    }
                }
                range.update(rule, RuleUpdateFlag::fail);
             }
        }
    }

    long long process2(const Input& input)
    {
        long long res = 0;

        auto workflows = input.first;
        std::vector<PartRange> ranges;
        traverse(workflows, ranges, "in", PartRange());

        for (auto range : ranges) {
            res += range.getCount();
        }

        return res;
    }
}

std::pair<long long, long long> day19()
{
    auto rawInput = readInput("inputs/day19.txt");
    auto input = processInput(rawInput);
    return {process1(input), process2(input)};
}