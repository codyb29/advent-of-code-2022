#include "23day.h"

#include <iostream>
#include <list>
#include <unordered_map>
#include <utility>

#define NORTH 0
#define DIR 3
#define TILE_SIZE 12

const int dir[TILE_SIZE][2] = {{0, -1}, {1, -1}, {-1, -1}, {0, 1},
                               {1, 1},  {-1, 1}, {-1, 0},  {-1, -1},
                               {-1, 1}, {1, 0},  {1, -1},  {1, 1}};

int get_elf_count(
    const std::unordered_map<int, std::unordered_map<int, int>> &coor_pairs,
    const int x, const int y) {
  std::unordered_map<int, std::unordered_map<int, int>>::const_iterator
      x_coor_it = coor_pairs.find(x);
  if (x_coor_it != coor_pairs.end()) {
    std::unordered_map<int, int>::const_iterator y_coor_it =
        x_coor_it->second.find(y);
    if (y_coor_it != x_coor_it->second.end()) {
      return y_coor_it->second;
    }
  }
  return 0;
}

void unstable_diffusion(const std::vector<std::string> &puzzle_input) {
  int elf_count = 0;
  std::unordered_map<int, std::unordered_map<int, int>> tile_has_elf;
  for (std::vector<std::string>::size_type i = 0; i < puzzle_input.size();
       i++) {
    for (std::string::size_type j = 0; j < puzzle_input[i].length(); j++) {
      if (puzzle_input[i][j] == '#') {
        tile_has_elf[j][i] = 1;
        elf_count += 1;
      }
    }
  }

  int current_direction = NORTH;
  int x1 = 0, y1 = 0, y2 = puzzle_input.size() - 1,
      x2 = puzzle_input[y2].length() - 1;
  bool has_elves_to_move = true;
  int round_count = 0;
  while (has_elves_to_move) {
    std::unordered_map<int, std::list<std::pair<int, int>>> moving_elves;
    std::unordered_map<int, std::unordered_map<int, int>> conflict_elves;
    for (std::unordered_map<int, std::unordered_map<int, int>>::const_iterator
             it = tile_has_elf.begin();
         it != tile_has_elf.end(); ++it) {
      for (std::unordered_map<int, int>::const_iterator jt = it->second.begin();
           jt != it->second.end(); ++jt) {
        bool all_clear = true;
        int clear_dir = -1;
        int next_dir = current_direction;
        for (int i = 0; i < TILE_SIZE; i += DIR) {
          bool is_side_clear =
              !get_elf_count(tile_has_elf, it->first + dir[next_dir][0],
                             jt->first + dir[next_dir][1]) &&
              !get_elf_count(tile_has_elf, it->first + dir[next_dir + 1][0],
                             jt->first + dir[next_dir + 1][1]) &&
              !get_elf_count(tile_has_elf, it->first + dir[next_dir + 2][0],
                             jt->first + dir[next_dir + 2][1]);
          if (clear_dir == -1 && is_side_clear) {
            clear_dir = next_dir;
          }
          all_clear = all_clear && is_side_clear;
          next_dir = (next_dir + DIR) % TILE_SIZE;
        }

        if (!all_clear && clear_dir != -1) {
          int x = it->first + dir[clear_dir][0],
              y = jt->first + dir[clear_dir][1];
          if (get_elf_count(conflict_elves, x, y) == 0) {
            moving_elves[clear_dir].emplace_back(it->first, jt->first);
            conflict_elves[x][y] = 1;
          } else {
            conflict_elves[x][y] += 1;
          }
        }
      }
    }

    has_elves_to_move = moving_elves.size() > 0;
    for (std::unordered_map<int, std::list<std::pair<int, int>>>::const_iterator
             it = moving_elves.begin();
         it != moving_elves.end(); ++it) {
      for (std::list<std::pair<int, int>>::const_iterator jt =
               it->second.begin();
           jt != it->second.end(); ++jt) {
        int x = jt->first + dir[it->first][0],
            y = jt->second + dir[it->first][1];
        // if 1 and only 1 elf is moving to designated spot, safely move.
        if (get_elf_count(conflict_elves, x, y) == 1) {
          tile_has_elf[jt->first].erase(
              tile_has_elf[jt->first].find(jt->second));
          tile_has_elf[x][y] = 1;

          // update boundaries as necessary
          if (x < x1) {
            x1 = x;
          } else if (x > x2) {
            x2 = x;
          }

          if (y < y1) {
            y1 = y;
          } else if (y > y2) {
            y2 = y;
          }
        }
      }
    }

    // end of round
    current_direction = (current_direction + DIR) % TILE_SIZE;
    round_count += 1;
    if (round_count == 10) {
      std::cout << "Empty ground tiles the rectangle contains after 10 rounds: "
                << ((((x2 + 1) - x1) * ((y2 + 1) - y1)) - elf_count)
                << std::endl;
    }
  }

  std::cout << "The number of the first round where no Elf moves: "
            << round_count << std::endl;
}