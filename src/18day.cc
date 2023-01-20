#include "18day.h"

#include <iostream>
#include <sstream>
#include <unordered_set>

#define WATER 0
#define LAVA 1
#define AIR_POCKET 2

std::vector<std::vector<std::vector<unsigned char>>>
build_lava_map(const std::vector<std::string> &puzzle_input) {
  std::vector<std::vector<std::vector<unsigned char>>> lava_map;
  unsigned x = 0, y = 0, z = 0;
  char comma = ',';
  for (std::vector<std::string>::const_iterator it = puzzle_input.begin();
       it != puzzle_input.end(); ++it) {
    std::stringstream ss(*it);
    ss >> x >> comma >> y >> comma >> z;
    while (lava_map.size() <= z) {
      lava_map.emplace_back(y + 1, std::vector<unsigned char>(x + 1, WATER));
    }
    while (lava_map[z].size() <= y) {
      lava_map[z].emplace_back(x + 1, WATER);
    }
    while (lava_map[z][y].size() <= x) {
      lava_map[z][y].emplace_back(WATER);
    }
    lava_map[z][y][x] = LAVA;
  }
  return lava_map;
}

bool is_in_bounds(std::vector<std::vector<std::vector<unsigned char>>> map,
                  unsigned x, unsigned y, unsigned z) {
  return z < map.size() && y < map[z].size() && x < map[z][y].size();
}

bool mark_air_packets(std::vector<std::vector<std::vector<unsigned char>>> &map,
                      std::unordered_set<std::string> &visited, int x, int y,
                      int z) {
  if (is_in_bounds(map, x, y, z)) {
    bool is_air = true;
    std::string key =
        std::to_string(x) + ',' + std::to_string(y) + ',' + std::to_string(z);
    if (!map[z][y][x] && visited.find(key) == visited.end()) {
      visited.insert(key);
      is_air = mark_air_packets(map, visited, x - 1, y, z);
      is_air = is_air && mark_air_packets(map, visited, x + 1, y, z);
      is_air = is_air && mark_air_packets(map, visited, x, y - 1, z);
      is_air = is_air && mark_air_packets(map, visited, x, y + 1, z);
      is_air = is_air && mark_air_packets(map, visited, x, y, z - 1);
      is_air = is_air && mark_air_packets(map, visited, x, y, z + 1);
    }

    return is_air;
  }
  return false;
}

bool is_side_exposed(std::vector<std::vector<std::vector<unsigned char>>> map,
                     unsigned x, unsigned y, unsigned z) {
  return !(is_in_bounds(map, x, y, z) && map[z][y][x] != WATER);
}

unsigned long get_lava_surface_area(
    const std::vector<std::vector<std::vector<unsigned char>>> &lava_map) {
  unsigned long surface_area = 0;
  for (std::vector<std::vector<std::vector<bool>>>::size_type z = 0;
       z < lava_map.size(); z++) {
    for (std::vector<std::vector<bool>>::size_type y = 0;
         y < lava_map[z].size(); y++) {
      for (std::vector<bool>::size_type x = 0; x < lava_map[z][y].size(); x++) {
        if (lava_map[z][y][x]) {
          // left
          if (x == 0 || is_side_exposed(lava_map, x - 1, y, z)) {
            surface_area += 1;
          }
          // right
          if (is_side_exposed(lava_map, x + 1, y, z)) {
            surface_area += 1;
          }
          // y up
          if (y == 0 || is_side_exposed(lava_map, x, y - 1, z)) {
            surface_area += 1;
          }
          // y down
          if (is_side_exposed(lava_map, x, y + 1, z)) {
            surface_area += 1;
          }
          // z up
          if (z == 0 || is_side_exposed(lava_map, x, y, z - 1)) {
            surface_area += 1;
          }
          // z down
          if (is_side_exposed(lava_map, x, y, z + 1)) {
            surface_area += 1;
          }
        }
      }
    }
  }
  return surface_area;
}

unsigned long
get_lava_surface_area_pt1(const std::vector<std::string> &puzzle_input) {
  return get_lava_surface_area(build_lava_map(puzzle_input));
}

unsigned long
get_lava_surface_area_pt2(const std::vector<std::string> &puzzle_input) {
  std::vector<std::vector<std::vector<unsigned char>>> lava_map =
      build_lava_map(puzzle_input);
  for (std::vector<std::vector<std::vector<bool>>>::size_type z = 0;
       z < lava_map.size(); z++) {
    for (std::vector<std::vector<bool>>::size_type y = 0;
         y < lava_map[z].size(); y++) {
      for (std::vector<bool>::size_type x = 0; x < lava_map[z][y].size(); x++) {
        if (!lava_map[z][y][x]) {
          std::unordered_set<std::string> visited;
          if (mark_air_packets(lava_map, visited, x, y, z)) {
            unsigned a = 0, b = 0, c = 0;
            char comma = ',';
            for (std::unordered_set<std::string>::const_iterator it =
                     visited.begin();
                 it != visited.end(); ++it) {
              std::stringstream ss(*it);
              ss >> a >> comma >> b >> comma >> c;
              lava_map[c][b][a] = AIR_POCKET;
            }
          }
        }
      }
    }
  }
  return get_lava_surface_area(lava_map);
}