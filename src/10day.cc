#include <vector>
#include <string>
#include <iostream>

#include "10day.h"

void CathodeRayTube::DecodeMessage(std::vector<std::string> instructions) {
    bool is_ok = true;
    int X = 1, pending_number = 0;
    std::vector<std::string>::const_iterator instruction = instructions.begin();
    for (unsigned int i = 0; i <= 240; i++) {
        if (i != 0 && i % 40 == 0) {
            std::cout << std::endl;
        }

        int pos = i % 40;
        std::cout << ((X - 1 <= pos && pos <= X + 1) ? '#' : '.');
        if (is_ok) {
            if (instruction->substr(0, 4) == "addx") {
                // assign next pending number
                pending_number = std::stoi(instruction->substr(5));
                is_ok = false;
            } else {
                pending_number = 0;
            }
            ++instruction;
        } else {
            X += pending_number;
            is_ok = true;
        }
    }

    std::cout << std::endl;
}

int CathodeRayTube::GetSignalStrength(std::vector<std::string> instructions) {
    int total = 0, X = 1, pending_number = 0;
    int current_cycle = 1, interested_cycles = 20;
    std::vector<std::string>::const_iterator instruction = instructions.begin();
    bool is_ok = true;

    while (instruction != instructions.end()) {
        if (current_cycle == interested_cycles) {
            total += (current_cycle * X);
            interested_cycles += 40;
        }

        if (is_ok) {
            if (instruction->substr(0, 4) == "addx") {
                // assign next pending number
                pending_number = std::stoi(instruction->substr(5));
                is_ok = false;
            } else {
                pending_number = 0;
            }
            ++instruction;
        } else {
            X += pending_number;
            is_ok = true;
        }
        current_cycle += 1;
    }
    return total;
}

