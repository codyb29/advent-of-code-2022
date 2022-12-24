#ifndef DAY11_H
#define DAY11_H

#include <cmath>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

class Monkey {
    std::queue<long>* items;
    std::vector<Monkey>* neighbors;
    bool is_careless;
    long divisible;
    long multiplier;
    bool has_multiplier;
    long gcd;

    long PlayWithItem(long item);

   public:
    Monkey(std::vector<long> worry_levels, long division_by,
           bool careless = false, long multiply = 0);
    void SetGCD(long GCD);
    void AddNeighbor(const Monkey& monkey);
    bool HasItems();
    void ThrowItem(long item);
    void InspectItem();
};

class MonkeyInTheMiddle {
   public:
    static long long GetMonkeyBusinessLevelForRounds(
        std::vector<Monkey> monkeys, int rounds);
    static std::vector<Monkey> MakeMonkeys(
        std::vector<std::string> puzzle_input);
};

#endif

