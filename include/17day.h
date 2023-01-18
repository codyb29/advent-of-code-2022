#ifndef DAY17_H
#define DAY17_H

#include <string>
#include <vector>

std::vector<signed char>
get_jet_pattern(const std::string arrow_representation);
unsigned long get_rock_tower_height(const std::vector<signed char> &jet_pattern,
                                    const unsigned long &rocks);

#endif