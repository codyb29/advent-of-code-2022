#ifndef DAY19_H
#define DAY19_H

#include <string>
#include <vector>

unsigned long
get_sum_of_all_blueprint_quality(const std::vector<std::string> &puzzle_input,
                                 unsigned minutes);

unsigned long get_product_of_top_3(const std::vector<std::string> &puzzle_input,
                                   unsigned minutes);

#endif