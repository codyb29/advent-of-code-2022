#include "22day.h"

#include <iostream>

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
  return 0 > y || static_cast<unsigned>(y) >= map.size() || 0 > x ||
         static_cast<unsigned>(x) >= map[y].length() || map[y][x] == ' ';
}

long monkey_map_pt1(const std::vector<std::string> &puzzle_input) {
  std::vector<std::string> map = build_map(puzzle_input);
  const std::vector<std::string> dir = build_directions(puzzle_input.back());
  const std::vector<std::pair<int, int>> dir_map = {
      {1, 0}, {0, 1}, {-1, 0}, {0, -1}};
  std::vector<char> debug = {'>', 'v', '<', '^'};

  int curr_dir = 0;
  int x = 0;
  int y = 0;
  while (map.front()[x] == ' ') {
    x += 1;
  }
  int count = 0;
  for (std::vector<std::string>::const_iterator it = dir.begin();
       it != dir.end(); ++it) {
    // std::cout << count << ", " << dir.size() << std::endl;
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
          map[y][x] = debug[curr_dir];
        }
        distance += 1;
      }

      // std::cout << "--------------------------------------" << std::endl;
      // if we don't have wall, just use modding technique.
      // if (map[ty][tx] != '#') {
      //   std::cout << "don't think this will ever be an issue..." <<
      //   std::endl; x = ((tx + (dir_map[curr_dir].first * steps)) % distance);
      //   y = ((ty + (dir_map[curr_dir].second * steps)) % distance);
      // if we have a wall, take the minimum between distance travel and
      // distance to wall.
      // }
    }
    count += 1;
  }

  // for (std::vector<std::string>::const_iterator it = map.begin();
  //      it != map.end(); ++it) {
  //   std::cout << *it << std::endl;
  // }
  return (1000 * (y + 1)) + (4 * (x + 1)) + curr_dir;
}