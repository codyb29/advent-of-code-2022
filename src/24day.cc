#include "24day.h"
#include "limits.h"

#include <array>
#include <iostream>
#include <queue>
#include <set>
#include <tuple>

struct position {
  int x;
  int y;
  unsigned long min;
};

struct blizzards {
  std::vector<std::vector<std::vector<bool>>> has_blizzard_vert;
  std::vector<std::vector<std::vector<bool>>> has_blizzard_horz;
};

const std::array<std::array<int, 2>, 4> side = {
    {{0, -1}, {1, 0}, {0, 1}, {-1, 0}}};

blizzards build_blizzards(const std::vector<std::string> &map) {
  // y -> x -> minute
  std::vector<std::vector<std::vector<bool>>> has_blizzard_vert(
      map.size(),
      std::vector<std::vector<bool>>(map.front().size(),
                                     std::vector<bool>(map.size(), false)));
  std::vector<std::vector<std::vector<bool>>> has_blizzard_horz(
      map.size(),
      std::vector<std::vector<bool>>(
          map.front().size(), std::vector<bool>(map.front().size(), false)));

  // build up the table
  const std::vector<bool>::size_type VERT_MAX = map.size() - 2;
  const std::vector<bool>::size_type HORZ_MAX = map.front().size() - 2;
  for (std::vector<std::string>::size_type i = 0; i < map.size(); ++i) {
    for (std::string::size_type j = 0; j != map[i].size(); j++) {
      std::string::size_type y = i, x = j;
      // wall offset iterations
      switch (map[i][j]) {
      case '^':
        for (std::vector<bool>::size_type k = 0; k < VERT_MAX; k++) {
          has_blizzard_vert[y][j][k] = true;
          y -= 1;
          if (map[y][j] == '#') {
            y = map.size() - 2;
          }
        }
        break;
      case '>':
        for (std::vector<bool>::size_type k = 0; k < HORZ_MAX; k++) {
          has_blizzard_horz[i][x][k] = true;
          x += 1;
          if (map[i][x] == '#') {
            x = 1;
          }
        }
        break;
      case 'v':
        for (std::vector<bool>::size_type k = 0; k < VERT_MAX; k++) {
          has_blizzard_vert[y][j][k] = true;
          y += 1;
          if (map[y][j] == '#') {
            y = 1;
          }
        }
        break;
      case '<':
        for (std::vector<bool>::size_type k = 0; k < HORZ_MAX; k++) {
          has_blizzard_horz[i][x][k] = true;
          x -= 1;
          if (map[i][x] == '#') {
            x = map[i].size() - 2;
          }
        }
        break;
      default:
        break;
      }
    }
  }
  return {has_blizzard_vert, has_blizzard_horz};
}

unsigned long get_shortest_path(const std::vector<std::string> &map,
                                blizzards bl, position start, const int GOAL_X,
                                const int GOAL_Y,
                                const std::vector<bool>::size_type VERT_MAX,
                                const std::vector<bool>::size_type HORZ_MAX) {
  unsigned long fewest_min = ULONG_MAX;
  std::set<std::tuple<int, int, unsigned long>> visited;
  std::queue<position> queue;
  queue.push(start);
  visited.insert(std::make_tuple(start.x, start.y, start.min));
  while (!queue.empty()) {
    position curr = queue.front();
    queue.pop();
    if (curr.y == GOAL_Y && curr.x == GOAL_X && curr.min < fewest_min) {
      fewest_min = curr.min;
      // don't bother if we're past our best.
    } else if (curr.min < fewest_min) {
      unsigned long elapse_min = curr.min + 1;
      std::vector<bool>::size_type lookup_vert_min = elapse_min % VERT_MAX;
      std::vector<bool>::size_type lookup_horz_min = elapse_min % HORZ_MAX;
      // check four sides to see where we can move to.
      for (std::array<std::array<int, 2>, 4>::const_iterator side_it =
               side.begin();
           side_it != side.end(); ++side_it) {
        int x = curr.x + side_it->at(0);
        int y = curr.y + side_it->at(1);
        if (0 <= y && y < static_cast<int>(map.size()) && 0 <= x &&
            x < static_cast<int>(map[y].size()) && map[y][x] != '#' &&
            visited.find(std::make_tuple(x, y, elapse_min)) == visited.end()) {
          // if in bounds and not wall, make sure no blizzards are coming.
          if (!bl.has_blizzard_vert[y][x][lookup_vert_min] &&
              !bl.has_blizzard_horz[y][x][lookup_horz_min]) {
            queue.push({x, y, elapse_min});
            visited.insert(std::make_tuple(x, y, elapse_min));
          }
        }
      }
      // It may also be optimal to not move, queue it up if no blizzards coming
      if (!bl.has_blizzard_vert[curr.y][curr.x][lookup_vert_min] &&
          !bl.has_blizzard_horz[curr.y][curr.x][lookup_horz_min] &&
          visited.find(std::make_tuple(curr.x, curr.y, elapse_min)) ==
              visited.end()) {
        queue.push({curr.x, curr.y, elapse_min});
        visited.insert(std::make_tuple(curr.x, curr.y, elapse_min));
      }
    }
  }
  return fewest_min;
}

unsigned long blizzard_basin_pt1(const std::vector<std::string> &map) {
  blizzards bl = build_blizzards(map);
  // position start, int goal_x, int goal_y
  return get_shortest_path(map, bl, {1, 0, 0}, map.front().size() - 2,
                           map.size() - 1, map.size() - 2,
                           map.front().size() - 2);
}

unsigned long blizzard_basin_pt2(const std::vector<std::string> &map) {
  const std::vector<bool>::size_type VERT_MAX = map.size() - 2;
  const std::vector<bool>::size_type HORZ_MAX = map.front().size() - 2;
  blizzards bl = build_blizzards(map);

  // position start, int goal_x, int goal_y
  const int START_X = 1;
  const int START_Y = 0;
  const int GOAL_Y = map.size() - 1;
  const int GOAL_X = map[GOAL_Y].size() - 2;
  unsigned long duration1 = get_shortest_path(
      map, bl, {START_X, START_Y, 0}, GOAL_X, GOAL_Y, VERT_MAX, HORZ_MAX);
  unsigned long duration2 =
      get_shortest_path(map, bl, {GOAL_X, GOAL_Y, duration1}, START_X, START_Y,
                        VERT_MAX, HORZ_MAX) -
      duration1;
  unsigned long duration3 =
      get_shortest_path(map, bl, {START_X, START_Y, duration1 + duration2},
                        GOAL_X, GOAL_Y, VERT_MAX, HORZ_MAX) -
      duration1 - duration2;
  return duration1 + duration2 + duration3;
}