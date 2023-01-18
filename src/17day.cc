#include "17day.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <utility>

#define X_MAX 6
#define X_MIN 0
#define Y_MIN 0

// create a function that switch cases below
// accepts bottom y coordinate to create shape above and which shape to make
std::vector<std::pair<long, long>> spawn_shape(const long &shape_id,
                                               const unsigned long &y) {
  std::vector<std::pair<long, long>> shape;
  switch (shape_id) {
    // horizontal straight
  case 0:
    shape.emplace_back(2L, y);
    shape.emplace_back(3L, y);
    shape.emplace_back(4L, y);
    shape.emplace_back(5L, y);
    break;
    // plus
  case 1:
    shape.emplace_back(2L, y + 1);
    shape.emplace_back(3L, y + 1);
    shape.emplace_back(4L, y + 1);
    shape.emplace_back(3L, y);
    shape.emplace_back(3L, y + 2);
    break;
    // backwards L
  case 2:
    shape.emplace_back(2L, y);
    shape.emplace_back(3L, y);
    shape.emplace_back(4L, y);
    shape.emplace_back(4L, y + 1);
    shape.emplace_back(4L, y + 2);
    break;
    // vertical straight
  case 3:
    shape.emplace_back(2L, y);
    shape.emplace_back(2L, y + 1);
    shape.emplace_back(2L, y + 2);
    shape.emplace_back(2L, y + 3);
    break;
    // square
  case 4:
    shape.emplace_back(2L, y);
    shape.emplace_back(2L, y + 1);
    shape.emplace_back(3L, y);
    shape.emplace_back(3L, y + 1);
    break;
  default:
    throw std::logic_error("Error: The given shape ID does not exist.");
  }

  return shape;
}

std::vector<signed char>
get_jet_pattern(const std::string arrow_representation) {
  // create vector to return out to.
  std::vector<signed char> jet_pattern;
  // loop through every char in the arrow representation
  for (char arrow : arrow_representation) {
    // if the arrow is >, add to vector as 1
    // otherwise, arrow is <, add to vector as -1
    jet_pattern.push_back(arrow == '>' ? 1 : -1);
  }

  // return the converted vector.
  return jet_pattern;
}

struct state {
  std::vector<signed char>::size_type dir;
  unsigned long shape_id;
  unsigned long rocks;

  state(std::vector<signed char>::size_type dir, unsigned long shape_id,
        unsigned long rocks) {
    this->dir = dir;
    this->shape_id = shape_id;
    this->rocks = rocks;
  }
};

bool is_space_occupied(const std::pair<long, long> &point,
                       const std::vector<std::vector<bool>> &history) {
  return point.second < static_cast<long>(history.size()) &&
         point.first < static_cast<long>(history[point.second].size()) &&
         history[point.second][point.first];
}

unsigned long get_rock_tower_height(const std::vector<signed char> &jet_pattern,
                                    const unsigned long &rocks) {
  std::vector<state> memo;
  // keep a pointer to what position we are in jet pattern
  std::vector<signed char>::size_type dir = 0;

  // keep a pointer to which shape we're making next
  unsigned long shape_id = 0;
  // keep track of a 2-d array that will store the coordinates that are
  // occupied.
  std::vector<std::vector<bool>> history;
  // loop through "rocks" times start
  unsigned long rock_count = 0;
  state *cycle_start_state = nullptr;
  std::vector<state>::size_type start = 0;
  while (rock_count < rocks && cycle_start_state == nullptr) {
    // create the next shape by calling shape maker below with shape pointer
    // and answer + 3
    std::vector<std::pair<long, long>> shape =
        spawn_shape(shape_id % 5, history.size() + 3);

    bool can_move_down = true;
    // while all coordinates will not move into floor or existing rock start
    while (can_move_down) {
      // check that wall boundary requirements are met and existing rock does
      // not exist
      signed char pattern = jet_pattern[dir % jet_pattern.size()];
      std::vector<std::pair<long, long>> shift_shape = shape;
      // all coordinates are either not in bounds or never seen.
      // move all the coordinates to the next direction it's blowing
      for (std::pair<long, long> &point : shift_shape) {
        point.first += pattern;
      }
      bool is_clear = true;
      // loop through all the coordinates
      for (std::pair<long, long> point : shift_shape) {
        // ensure coordinate isn't next to wall and never seen in history.
        if (point.first < X_MIN || point.first > X_MAX ||
            is_space_occupied(point, history)) {
          is_clear = false;
        }
      }
      if (is_clear) {
        shape = shift_shape;
      }

      // now check floor and existing rock boundary requirements are met
      // loop through all the coordinates
      for (std::pair<long, long> point : shape) {
        point.second -= 1;
        // ensure coordinate isn't next to floor and never seen in history.
        if (point.second < Y_MIN || is_space_occupied(point, history)) {
          can_move_down = false;
        }
      }
      if (can_move_down) {
        // if above is met, move down
        for (std::pair<long, long> &point : shape) {
          point.second -= 1;
        }
      }
      // otherwise, we collided with rock or hit the floor. stop.
      dir += 1;
    }

    // loop through all coordinates and find the highest y point to update ans
    // to. Also insert coordinates into history.
    for (std::pair<int, int> point : shape) {
      // check that the history has enough height to keep track
      while (static_cast<int>(history.size()) <= point.second) {
        history.emplace_back(X_MAX + 1, false);
      }
      history[point.second][point.first] = true;
    }
    while (memo.size() < history.size()) {
      memo.emplace_back(0, 0, 0);
    }
    rock_count += 1;

    memo[history.size() - 1] =
        state(dir % jet_pattern.size(), shape_id % 5, rock_count);
    start = 0;
    while (start < memo.size() - 1 && cycle_start_state == nullptr) {
      if (memo[start].dir == (dir % jet_pattern.size()) &&
          memo[start].shape_id == (shape_id % 5) &&
          std::equal(history[history.size() - 1].begin(),
                     history[history.size() - 1].end(),
                     history[start].begin())) {
        cycle_start_state = &memo[start];
      }
      start += 1;
    }

    shape_id += 1;
  }

  // simulated faster than detecting a pattern
  if (rock_count == rocks) {
    return history.size();
  }

  unsigned long rocks_remaining = rocks - rock_count;
  unsigned long height_of_cycle = history.size() - start;
  unsigned long rocks_in_cycle =
      memo[history.size() - 1].rocks - cycle_start_state->rocks;

  unsigned long cycle_count = rocks_remaining / rocks_in_cycle;
  unsigned long remainder =
      (rocks_remaining % rocks_in_cycle) + cycle_start_state->rocks;

  std::vector<state>::size_type remaining_height = start;
  while (remaining_height < memo.size() &&
         memo[remaining_height].rocks != remainder) {
    remaining_height += 1;
  }

  return history.size() + (height_of_cycle * cycle_count) +
         (remaining_height - (start - 1));
}
