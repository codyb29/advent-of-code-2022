#include "14day.h"
#include <iostream>
#include <sstream>
#include <utility>

int RegolithReservoir::count_resting_sand(std::vector<std::string> rock_map) {
  int lowest_rock = 0;
  std::unordered_set<std::string> occupied =
      ConvertToCoordinatePairs(rock_map, lowest_rock);
  int rocks_count = occupied.size();

  int y = 0, x = 0;
  while (y <= lowest_rock) {
    x = 500, y = 0;
    int px = 0, py = 0;
    while (y <= lowest_rock && (x != px || y != py)) {
      px = x;
      py = y;
      // Attempt to move down
      if (occupied.find(get_key(x, y + 1)) == occupied.end()) {
        y += 1;
        // Attempt to move down right
      } else if (occupied.find(get_key(x - 1, y + 1)) == occupied.end()) {
        x -= 1;
        y += 1;
        // Attempt to move down left
      } else if (occupied.find(get_key(x + 1, y + 1)) == occupied.end()) {
        x += 1;
        y += 1;
      }
    }
    if (y <= lowest_rock) {
      occupied.insert(get_key(x, y));
    }
  }

  return occupied.size() - rocks_count;
}

int RegolithReservoir::count_resting_sand_with_floor(
    std::vector<std::string> rock_map) {
  int lowest_rock = 0;
  std::unordered_set<std::string> occupied =
      ConvertToCoordinatePairs(rock_map, lowest_rock);
  lowest_rock += 1;
  int rocks_count = occupied.size();

  int y = 0, x = 0;
  while (x != 500 || y != 0) {
    x = 500, y = 0;
    int px = 0, py = 0;
    while (x != px || y != py) {
      px = x;
      py = y;
      if (y < lowest_rock) {
        // Attempt to move down
        if (occupied.find(get_key(x, y + 1)) == occupied.end()) {
          y += 1;
          // Attempt to move down right
        } else if (occupied.find(get_key(x - 1, y + 1)) == occupied.end()) {
          x -= 1;
          y += 1;
          // Attempt to move down left
        } else if (occupied.find(get_key(x + 1, y + 1)) == occupied.end()) {
          x += 1;
          y += 1;
        }
      }
    }
    occupied.insert(get_key(x, y));
  }

  return occupied.size() - rocks_count;
}

std::string RegolithReservoir::get_key(int x, int y) {
  return std::to_string(x) + "," + std::to_string(y);
}

std::unordered_set<std::string>
RegolithReservoir::ConvertToCoordinatePairs(std::vector<std::string> rock_map,
                                            int &min) {
  std::unordered_set<std::string> rock_coordinates;
  for (std::vector<std::string>::const_iterator path = rock_map.begin();
       path != rock_map.end(); ++path) {
    // create a stringstream from the input string
    std::istringstream iss(*path);
    int px = 0;
    int py = 0;

    // use getline to split the string on the "->" delimiter
    std::string item;
    while (iss) {
      int x;
      int y;
      // Extract the values separated by ",".
      char delimiter;
      iss >> x >> delimiter >> y;
      min = std::max(min, y);

      // Ignore the "->" separator.
      std::string separator;
      std::getline(iss, separator, '>');

      // Add the path to the set.
      if (px == x) {
        int min = std::min(py, y);
        int max = std::max(py, y);
        for (int i = min; i <= max; i++) {
          rock_coordinates.insert(get_key(x, i));
        }
      } else if (py == y) {
        int min = std::min(px, x);
        int max = std::max(px, x);
        for (int i = min; i <= max; i++) {
          rock_coordinates.insert(get_key(i, y));
        }
      }
      px = x;
      py = y;
    }
  }

  return rock_coordinates;
}
