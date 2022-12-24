#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#define THREE_ELVES 3

const std::vector<std::string> ConvertInputFileToVector(
    const std::string file_name) {
  std::vector<std::string> input_buffer;
  std::ifstream in(file_name);

  std::string line;
  while (std::getline(in, line)) {
    input_buffer.push_back(line);
  }

  return input_buffer;
}

bool ConvertStringToInt(const std::string string_to_convert, int& output) {
  if (!string_to_convert.empty()) {
    try {
      output = std::stoi(string_to_convert);
      return true;
    } catch (std::out_of_range& err) {
      std::cout << err.what() << std::endl;
    }
  }

  return false;
}

int ElfWithMostCalories(const std::vector<std::string>& elf_calories) {
  int max_calories_so_far = 0;
  int current_elf_calories = 0;
  for (std::vector<std::string>::const_iterator itr = elf_calories.begin();
       itr != elf_calories.end(); ++itr) {
    std::string calories_as_string = *itr;
    int calories;
    bool isNumber = ConvertStringToInt(*itr, calories);
    if (isNumber) {
      current_elf_calories += calories;
    }
    if (!isNumber || itr + 1 == elf_calories.end()) {
      max_calories_so_far = std::max(max_calories_so_far, current_elf_calories);
      current_elf_calories = 0;
    }
  }
  return max_calories_so_far;
}

int ElvesWithMostCalories(const std::vector<std::string>& elf_calories) {
  std::vector<int> all_totals;
  std::priority_queue<int, std::vector<int>, std::greater<int> > pq;
  int current_elf_calories = 0;
  for (std::vector<std::string>::const_iterator itr = elf_calories.begin();
       itr != elf_calories.end(); ++itr) {
    int calories;
    bool isNumber = ConvertStringToInt(*itr, calories);
    if (isNumber) {
      current_elf_calories += calories;
    }
    if (!isNumber || itr + 1 == elf_calories.end()) {
      if (pq.size() < THREE_ELVES) {
        pq.push(current_elf_calories);
      } else if (current_elf_calories > pq.top()) {
        pq.pop();
        pq.push(current_elf_calories);
      }
      all_totals.push_back(current_elf_calories);
      current_elf_calories = 0;
    }
  }

  int elves_with_most_calories = 0;
  while (!pq.empty()) {
    elves_with_most_calories += pq.top();
    pq.pop();
  }
  return elves_with_most_calories;
}

