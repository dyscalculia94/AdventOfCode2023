#include "day07.h"

#include <iostream>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <cmath>


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

    enum class Type : int
    {
        high,
        pair,
        two,
        three,
        full,
        four,
        five
    };

    std::string getTypeName(Type type)
    {
        switch(type) {
            case Type::five: return "five";
            case Type::four: return "four";
            case Type::full: return "full";
            case Type::three: return "three";
            case Type::two: return "two";
            case Type::pair: return "pair";
            case Type::high: return "high";
        }

        return "";
    }

    struct Hand
    {
        Hand(std::string cards) : hand(modify(cards)) { type = getType(hand, false); }

        std::string modify(std::string cards)
        {
            std::replace(cards.begin(), cards.end(), 'A', 'E');
            std::replace(cards.begin(), cards.end(), 'K', 'D');
            std::replace(cards.begin(), cards.end(), 'Q', 'C');
            std::replace(cards.begin(), cards.end(), 'J', 'B');
            std::replace(cards.begin(), cards.end(), 'T', 'A');
            return cards;
        }

        Type getType(std::string cards, bool useJoker)
        {
            std::sort(cards.begin(), cards.end());

            char cur = cards[0];
            int count = 1;
            int jokers = 0;
            std::vector<int> counts;
            for (int i = 1; i < cards.size(); i++) {
                if (cur != cards[i]) {
                    if (useJoker && cur == '1') {
                        jokers = count;
                    }
                    else {
                        counts.push_back(count);
                    }
                    
                    cur = cards[i];
                    count = 1;
                }
                else {
                    count++;
                }
            }
            counts.push_back(count);
            
            std::sort(counts.begin(), counts.end(), std::greater<>());
            counts[0] += jokers;

            if (counts[0] == 5) {
                return Type::five;
            }

            if (counts[0] == 4) {
                return Type::four;
            }

            if (counts[0] == 3) {
                if (counts[1] == 2) {
                    return Type::full;
                }
                return Type::three;
            }
            
            if (counts[0] == 2) {
                if (counts[1] == 2) {
                    return Type::two;
                }
                return Type::pair;
            }

            return Type::high;
        }

        void addJoker()
        {
            std::replace(hand.begin(), hand.end(), 'B', '1');

            type = getType(hand, true);
        }

        friend std::ostream& operator<<(std::ostream& out, const Hand& h)
        {
            out << h.hand << " " << getTypeName(h.type);
            return out;
        }

        friend bool operator<(const Hand& a, const Hand& b)
        {
            return a.type == b.type ? a.hand < b.hand : a.type < b.type;
        }

        std::string hand;
        Type type;
    };

    std::vector<std::pair<Hand, int>> processInput(const std::vector<std::string>& input)
    {
        std::vector<std::pair<Hand, int>> res;

        for (auto line : input) {
            res.push_back({Hand(line.substr(0, line.find(" "))), std::stoi(line.substr(line.find(" ") + 1))});
        }

        return res;
    }

    int calculateWinnings(std::vector<std::pair<Hand, int>>& input)
    {
        int res = 0;

        std::sort(input.begin(), input.end(), [](const auto& a, const auto& b) { return a.first < b.first; });

        for (int i = 0; i < input.size(); i++) {
            res += (i + 1) * input[i].second;
        }

        return res;
    }

    int process1(std::vector<std::pair<Hand, int>> input)
    {
        return calculateWinnings(input);
    }

    int process2(std::vector<std::pair<Hand, int>> input)
    {
        for (auto& i : input) {
            i.first.addJoker();
        }

        return calculateWinnings(input);
    }
}

std::pair<int, int> day07()
{
    auto rawInput = readInput("inputs/day07.txt");
    auto input = processInput(rawInput);
    return {process1(input), process2(input)};
}