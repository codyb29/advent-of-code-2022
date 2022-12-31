#ifndef DAY14_H
#define DAY14_H

#include <string>
#include <unordered_set>
#include <vector>

class RegolithReservoir {
  static std::unordered_set<std::string>
  ConvertToCoordinatePairs(std::vector<std::string> rock_map, int &min);
  static std::string get_key(int x, int y);

public:
  static int count_resting_sand(std::vector<std::string> rock_map);
  static int count_resting_sand_with_floor(std::vector<std::string> rock_map);
};

#endif