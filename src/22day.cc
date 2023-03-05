#include "22day.h"

#include <map>

#define RIGHT 0
#define DOWN 1
#define LEFT 2
#define UP 3

const std::vector<std::string>
build_map(const std::vector<std::string> &puzzle_input) {
  // find out how much padding we need to apply after a short row.
  unsigned max_x = 0;
  std::vector<std::string>::const_iterator end_of_map_it =
      puzzle_input.end() - 2;
  for (std::vector<std::string>::const_iterator it = puzzle_input.begin();
       it != end_of_map_it; ++it) {
    if (max_x < it->length()) {
      max_x = it->length();
    }
  }

  unsigned end_of_map_idx = puzzle_input.size() - 2;
  // return corrected puzzle input with padding - the direction catalog at end.
  std::vector<std::string> map(end_of_map_idx, std::string(max_x, ' '));
  for (unsigned y = 0; y < end_of_map_idx; y++) {
    for (unsigned x = 0; x < puzzle_input[y].length(); x++) {
      if (puzzle_input[y][x]) {
        map[y][x] = puzzle_input[y][x];
      }
    }
  }

  return map;
}

const std::vector<std::string>
build_directions(const std::string given_directions) {
  std::vector<std::string> directions;
  std::string build_step_count = "";
  for (std::string::const_iterator it = given_directions.begin();
       it != given_directions.end(); ++it) {
    if (*it != 'L' && *it != 'R') {
      build_step_count += *it;
    } else {
      // place the step count
      directions.emplace_back(build_step_count);
      build_step_count = "";
      // place the direction itself
      directions.emplace_back(std::string(1, *it));
    }
  }

  // add pending potential number.
  if (build_step_count.length() > 0) {
    directions.emplace_back(build_step_count);
  }

  return directions;
}

bool is_out_of_bounds(int x, int y, const std::vector<std::string> map) {
  return 0 > y || y >= static_cast<int>(map.size()) || 0 > x ||
         x >= static_cast<int>(map[y].length()) || map[y][x] == ' ';
}

long monkey_map_pt1(const std::vector<std::string> &puzzle_input) {
  std::vector<std::string> map = build_map(puzzle_input);
  const std::vector<std::string> dir = build_directions(puzzle_input.back());
  const std::vector<std::pair<int, int>> dir_map = {
      {1, 0}, {0, 1}, {-1, 0}, {0, -1}};

  int curr_dir = 0;
  int x = 0;
  int y = 0;
  while (map.front()[x] == ' ') {
    x += 1;
  }
  int count = 0;
  for (std::vector<std::string>::const_iterator it = dir.begin();
       it != dir.end(); ++it) {
    if (*it == "L") {
      if (curr_dir > 0) {
        curr_dir -= 1;
      } else {
        curr_dir = 3;
      }
    } else if (*it == "R") {
      curr_dir = (curr_dir + 1) % 4;
    } else {
      // convert *it to long
      long steps = std::stol(*it);
      // determine max potential for direction
      // determine haswall and distance to wall, or distance to self.
      unsigned distance = 0;
      int tx = x;
      int ty = y;
      const int opposite = (curr_dir + 2) % 4;
      while (map[ty][tx] != '#' && distance < steps) {
        // traverse row/col
        tx += dir_map[curr_dir].first;
        ty += dir_map[curr_dir].second;
        // we hit out of bounds, turn around and find the other side.
        if (is_out_of_bounds(tx, ty, map)) {
          tx = x;
          ty = y;
          // while in bounds and not empty space, keep moving opposite
          // direction.
          while (0 <= ty && static_cast<unsigned>(ty) < map.size() && 0 <= tx &&
                 static_cast<unsigned>(tx) < map[ty].length() &&
                 map[ty][tx] != ' ') {
            tx += dir_map[opposite].first;
            ty += dir_map[opposite].second;
          }
          // bring back into the grid.
          tx += dir_map[curr_dir].first;
          ty += dir_map[curr_dir].second;
        }

        // increment our distance if we haven't hit the wall or went the whole
        // row/col, also make sure we aren't in the void.
        if (map[ty][tx] != '#') {
          x = tx;
          y = ty;
        }
        distance += 1;
      }
    }
    count += 1;
  }

  return (1000 * (y + 1)) + (4 * (x + 1)) + curr_dir;
}

// hardcoded unfortunately...
const std::map<std::tuple<int, int, int>, std::tuple<int, int, int>>
build_cube_map() {
  std::map<std::tuple<int, int, int>, std::tuple<int, int, int>> cube_map;

  for (int i = 0; i < 50; i++) {
    // top middle top
    cube_map[std::make_tuple(50 + i, -1, UP)] =
        std::make_tuple(0, 150 + i, RIGHT);
    // top right top
    cube_map[std::make_tuple(100 + i, -1, UP)] = std::make_tuple(i, 199, UP);
    // top right bottom
    cube_map[std::make_tuple(100 + i, 50, DOWN)] =
        std::make_tuple(99, 50 + i, LEFT);
    // middle left top
    cube_map[std::make_tuple(i, 99, UP)] = std::make_tuple(50, 50 + i, RIGHT);
    // middle middle bottom
    cube_map[std::make_tuple(50 + i, 150, DOWN)] =
        std::make_tuple(49, 150 + i, LEFT);
    // bottom left bottom
    cube_map[std::make_tuple(i, 200, DOWN)] = std::make_tuple(100 + i, 0, DOWN);

    // // top middle left
    cube_map[std::make_tuple(49, i, LEFT)] = std::make_tuple(0, 149 - i, RIGHT);
    // // top right right
    cube_map[std::make_tuple(150, i, RIGHT)] =
        std::make_tuple(99, 149 - i, LEFT);
    // middle middle left
    cube_map[std::make_tuple(49, 50 + i, LEFT)] = std::make_tuple(i, 100, DOWN);
    // middle middle right
    cube_map[std::make_tuple(100, 50 + i, RIGHT)] =
        std::make_tuple(100 + i, 49, UP);
    // middle left left
    cube_map[std::make_tuple(-1, 100 + i, LEFT)] =
        std::make_tuple(50, 49 - i, RIGHT);
    // middle middle right2
    cube_map[std::make_tuple(100, 100 + i, RIGHT)] =
        std::make_tuple(149, 49 - i, LEFT);
    // bottom left left
    cube_map[std::make_tuple(-1, 150 + i, LEFT)] =
        std::make_tuple(50 + i, 0, DOWN);
    // bottom left right
    cube_map[std::make_tuple(50, 150 + i, RIGHT)] =
        std::make_tuple(50 + i, 149, UP);
  }

  return cube_map;
}

long monkey_map_pt2(const std::vector<std::string> &puzzle_input) {
  std::vector<std::string> map = build_map(puzzle_input);
  const std::vector<std::string> dir = build_directions(puzzle_input.back());
  const std::map<std::tuple<int, int, int>, std::tuple<int, int, int>>
      cube_map = build_cube_map();
  const std::vector<std::pair<int, int>> dir_map = {
      {1, 0}, {0, 1}, {-1, 0}, {0, -1}};

  int curr_dir = 0;
  int x = 0;
  int y = 0;
  while (map.front()[x] == ' ') {
    x += 1;
  }
  for (std::vector<std::string>::const_iterator it = dir.begin();
       it != dir.end(); ++it) {
    if (*it == "L") {
      if (curr_dir > 0) {
        curr_dir -= 1;
      } else {
        curr_dir = 3;
      }
    } else if (*it == "R") {
      curr_dir = (curr_dir + 1) % 4;
    } else {
      // convert *it to long
      long steps = std::stol(*it);
      // determine max potential for direction
      // determine haswall and distance to wall, or distance to self.
      unsigned distance = 0;
      int tx = x;
      int ty = y;
      while (map[ty][tx] != '#' && distance < steps) {
        // traverse row/col
        tx += dir_map[curr_dir].first;
        ty += dir_map[curr_dir].second;
        int temp_dir = curr_dir;
        // we hit out of bounds, turn around and find the other side.
        if (is_out_of_bounds(tx, ty, map)) {
          std::map<std::tuple<int, int, int>,
                   std::tuple<int, int, int>>::const_iterator jt =
              cube_map.find(std::make_tuple(tx, ty, curr_dir));
          if (jt != cube_map.end()) {
            tx = std::get<0>(jt->second);
            ty = std::get<1>(jt->second);
            temp_dir = std::get<2>(jt->second);
          }
        }

        // increment our distance if we haven't hit the wall or went the whole
        // row/col, also make sure we aren't in the void.
        if (map[ty][tx] != '#') {
          x = tx;
          y = ty;
          curr_dir = temp_dir;
        }
        distance += 1;
      }
    }
  }

  return (1000 * (y + 1)) + (4 * (x + 1)) + curr_dir;
}