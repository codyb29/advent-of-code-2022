#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

#include "05day.h"

std::vector<std::stack<char>> SupplyStacks::ProcessCrates(
    const std::vector<std::string>& puzzle_input) {
    std::string::size_type number_of_crates =
        (puzzle_input[0].length() + 1) / 4;
    std::vector<std::stack<char>> crates(number_of_crates);
    for (std::vector<std::string>::size_type i = 7;
         i != (std::vector<std::string>::size_type) - 1; i--) {
        for (std::string::size_type j = 1; j < puzzle_input[i].length();
             j += 4) {
            if (puzzle_input[i][j] >= 'A' && puzzle_input[i][j] <= 'Z') {
                crates[j / 4].push(puzzle_input[i][j]);
            }
        }
    }
    // for (std::vector<std::stack<char>>::size_type i = 0; i < crates.size();
    //      i++) {
    //   std::cout << "crate #" << i << std::endl;
    //   while (!crates[i].empty()) {
    //     std::cout << crates[i].top() << ", ";
    //     crates[i].pop();
    //   }
    //   std::cout << std::endl;
    // }
    return crates;
}

std::vector<std::vector<int>> SupplyStacks::ProcessInstructions(
    const std::vector<std::string>& puzzle_input) {
    std::vector<std::vector<int>> instructions;
    std::vector<std::vector<int>>::size_type j = 0;
    for (std::vector<std::string>::size_type i = 10; i < puzzle_input.size();
         i++, j++) {
        instructions.push_back(std::vector<int>());
        std::string buffer;
        std::stringstream ss(puzzle_input[i]);
        while (ss >> buffer) {
            try {
                instructions[j].push_back(std::stoi(buffer));
            } catch (std::invalid_argument const& ex) {
                // std::cout << "std::invalid_argument::what(): " << ex.what()
                //           << std::endl;
            }
        }
    }

    // for (std::vector<std::vector<int>>::const_iterator itr =
    //          instructions.begin();
    //      itr != instructions.end(); ++itr) {
    //   for (std::vector<int>::const_iterator itr2 = itr->begin();
    //        itr2 != itr->end(); ++itr2) {
    //     std::cout << *itr2 << ", ";
    //   }
    //   std::cout << std::endl;
    // }
    return instructions;
}

std::string SupplyStacks::SimulateCrateMover9000(
    std::vector<std::stack<char>> crates,
    const std::vector<std::vector<int>>& instructions) {
    for (std::vector<std::vector<int>>::const_iterator it =
             instructions.begin();
         it != instructions.end(); ++it) {
        for (int i = 0; i < (*it)[0]; i++) {
            char move_crate = crates[(*it)[1] - 1].top();
            crates[(*it)[1] - 1].pop();
            crates[(*it)[2] - 1].push(move_crate);
        }
    }

    std::string crates_at_top = "";
    for (std::vector<std::stack<char>>::const_iterator it = crates.begin();
         it != crates.end(); ++it) {
        crates_at_top += it->top();
    }
    return crates_at_top;
}

std::string SupplyStacks::SimulateCrateMover9001(
    std::vector<std::stack<char>> crates,
    const std::vector<std::vector<int>>& instructions) {
    for (std::vector<std::vector<int>>::const_iterator it =
             instructions.begin();
         it != instructions.end(); ++it) {
        std::vector<char> crates_to_move;
        for (int i = 0; i < (*it)[0]; i++) {
            crates_to_move.push_back(crates[(*it)[1] - 1].top());
            crates[(*it)[1] - 1].pop();
        }

        for (std::vector<char>::size_type i = crates_to_move.size() - 1;
             i != (std::vector<char>::size_type) - 1; i--) {
            crates[(*it)[2] - 1].push(crates_to_move[i]);
        }
    }

    std::string crates_at_top = "";
    for (std::vector<std::stack<char>>::const_iterator it = crates.begin();
         it != crates.end(); ++it) {
        crates_at_top += it->top();
    }
    return crates_at_top;
}

