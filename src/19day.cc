#include "19day.h"

#include <cmath>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <utility>

#define ORE 0
#define CLAY 1
#define OBSIDIAN 2
#define GEODE 3
#define ALLOW_ALL_BOTS 15

struct robot_blueprint {
  unsigned ore;
  unsigned clay;
  std::pair<unsigned, unsigned> obsidian;
  std::pair<unsigned, unsigned> geode;
  unsigned expensive_bot;

  robot_blueprint(unsigned ore, unsigned clay,
                  std::pair<unsigned, unsigned> obsidian,
                  std::pair<unsigned, unsigned> geode, unsigned expensive_bot)
      : ore(ore), clay(clay), obsidian(obsidian), geode(geode),
        expensive_bot(expensive_bot) {}
};

std::vector<robot_blueprint>
read_blueprints(const std::vector<std::string> &puzzle_input) {
  std::vector<robot_blueprint> robot_blueprints;

  std::string buffer;
  char colon = ':';
  unsigned number_buffer;
  unsigned ore = 0, clay = 0, expensive = 0;
  std::pair<unsigned, unsigned> obsidian, geode;
  for (std::vector<std::string>::const_iterator blueprint =
           puzzle_input.begin();
       blueprint != puzzle_input.end(); ++blueprint) {
    std::stringstream ss(*blueprint);

    ss >> buffer >> number_buffer >> colon;
    for (unsigned char i = 0; i < 4; i++) {
      ss >> buffer;
    }
    ss >> ore >> buffer;
    for (unsigned char i = 0; i < 4; i++) {
      ss >> buffer;
    }
    ss >> clay >> buffer;
    for (unsigned char i = 0; i < 4; i++) {
      ss >> buffer;
    }
    ss >> obsidian.first >> buffer >> buffer >> obsidian.second >> buffer;
    for (unsigned char i = 0; i < 4; i++) {
      ss >> buffer;
    }
    ss >> geode.first >> buffer >> buffer >> geode.second >> buffer;

    expensive = ore;
    if (expensive < clay) {
      expensive = clay;
    }
    if (expensive < obsidian.first) {
      expensive = obsidian.first;
    }
    if (expensive < geode.first) {
      expensive = geode.first;
    }

    robot_blueprints.emplace_back(ore, clay, obsidian, geode, expensive);
  }

  return robot_blueprints;
}

unsigned long get_sum_of_blueprint_quality(unsigned minutes, unsigned ore,
                                           unsigned clay, unsigned obsidian,
                                           std::vector<unsigned> robot_count,
                                           unsigned char is_allowed,
                                           robot_blueprint &blueprint) {
  if (minutes == 0) {
    return 0;
  }

  unsigned elapse_min = minutes - 1;
  unsigned add_ore = ore + robot_count[ORE];
  unsigned add_clay = clay + robot_count[CLAY];
  unsigned add_obsidian = obsidian + robot_count[OBSIDIAN];
  unsigned long make_geode = 0;
  unsigned long make_obsidian = 0;
  unsigned long make_clay = 0;
  unsigned long make_ore = 0;
  unsigned char next_allowed = is_allowed;

  if (robot_count[ORE] < blueprint.geode.first ||
      robot_count[OBSIDIAN] < blueprint.geode.second) {
    if (is_allowed & 8 && blueprint.geode.first <= ore &&
        blueprint.geode.second <= obsidian) {
      std::vector<unsigned> next_robot_count(robot_count);
      next_robot_count[GEODE] += 1;
      make_geode =
          elapse_min + get_sum_of_blueprint_quality(
                           elapse_min, add_ore - blueprint.geode.first,
                           add_clay, add_obsidian - blueprint.geode.second,
                           next_robot_count, ALLOW_ALL_BOTS, blueprint);
      next_allowed &= 7; // 0111
    }

    if (is_allowed & 4 && robot_count[OBSIDIAN] < blueprint.geode.second &&
        blueprint.obsidian.first <= ore && blueprint.obsidian.second <= clay) {
      std::vector<unsigned> next_robot_count(robot_count);
      next_robot_count[OBSIDIAN] += 1;
      // minute, ore, and clay robots
      make_obsidian = get_sum_of_blueprint_quality(
          elapse_min, add_ore - blueprint.obsidian.first,
          add_clay - blueprint.obsidian.second, add_obsidian, next_robot_count,
          ALLOW_ALL_BOTS, blueprint);
      next_allowed &= 11; // 1011
    }

    if (is_allowed & 2 && robot_count[CLAY] < blueprint.obsidian.second &&
        blueprint.clay <= ore) {
      std::vector<unsigned> next_robot_count(robot_count);
      next_robot_count[CLAY] += 1;
      make_clay = get_sum_of_blueprint_quality(
          elapse_min, add_ore - blueprint.clay, add_clay, add_obsidian,
          next_robot_count, ALLOW_ALL_BOTS, blueprint);
      next_allowed &= 13; // 1101
    }

    if (is_allowed & 1 && blueprint.ore <= ore &&
        robot_count[ORE] < blueprint.expensive_bot) {
      std::vector<unsigned> next_robot_count(robot_count);
      next_robot_count[ORE] += 1;

      make_ore = get_sum_of_blueprint_quality(
          elapse_min, add_ore - blueprint.ore, add_clay, add_obsidian,
          next_robot_count, ALLOW_ALL_BOTS, blueprint);
      next_allowed &= 14; // 1110
    }
  } else if (is_allowed & 8) {
    make_geode = (elapse_min * (elapse_min + 1)) / 2;
    next_allowed &= 7; // 0111
  }

  unsigned long geode_obsidian = std::max(make_geode, make_obsidian);
  unsigned long clay_ore = std::max(make_clay, make_ore);
  unsigned long make_robot = std::max(geode_obsidian, clay_ore);
  return std::max(make_robot, get_sum_of_blueprint_quality(
                                  elapse_min, add_ore, add_clay, add_obsidian,
                                  robot_count, next_allowed, blueprint));
}

unsigned long
get_sum_of_all_blueprint_quality(const std::vector<std::string> &puzzle_input,
                                 unsigned minutes) {
  std::vector<robot_blueprint> robot_blueprints = read_blueprints(puzzle_input);
  unsigned long sum = 0;
  for (std::vector<robot_blueprint>::size_type i = 0;
       i < robot_blueprints.size(); i++) {
    std::vector<unsigned> robot_count = {1, 0, 0, 0};
    std::unordered_map<std::string, long> memo;
    int sum_of_blueprint_quality = get_sum_of_blueprint_quality(
        minutes, 0, 0, 0, robot_count, ALLOW_ALL_BOTS, robot_blueprints[i]);
    sum += ((i + 1) * sum_of_blueprint_quality);
  }
  return sum;
}

unsigned long get_product_of_top_3(const std::vector<std::string> &puzzle_input,
                                   unsigned minutes) {
  std::vector<robot_blueprint> robot_blueprints = read_blueprints(puzzle_input);
  unsigned long sum = 1;
  for (std::vector<robot_blueprint>::size_type i = 0;
       i < robot_blueprints.size() && i < 3; i++) {
    std::vector<unsigned> robot_count = {1, 0, 0, 0};
    std::unordered_map<std::string, long> memo;
    int sum_of_blueprint_quality = get_sum_of_blueprint_quality(
        minutes, 0, 0, 0, robot_count, ALLOW_ALL_BOTS, robot_blueprints[i]);
    sum *= sum_of_blueprint_quality;
  }
  return sum;
}
