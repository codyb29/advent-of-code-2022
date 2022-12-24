#ifndef DAY05_H
#define DAY05_H

#include <stack>

class SupplyStacks {
 public:
  static std::vector<std::stack<char>> ProcessCrates(
      const std::vector<std::string>& puzzle_input);
  static std::vector<std::vector<int>> ProcessInstructions(
      const std::vector<std::string>& puzzle_input);
 static std::string SimulateCrateMover9000(
      std::vector<std::stack<char>> crates,
      const std::vector<std::vector<int>>& instructions);
  static std::string SimulateCrateMover9001(
      std::vector<std::stack<char>> crates,
      const std::vector<std::vector<int>>& instructions);
};

#endif
