#ifndef DAY13_H
#define DAY13_H

#define RIGHT_ORDER "right order"
#define WRONG_ORDER "wrong order"
#define NO_ORDER "no order"

#include <list>
#include <string>
#include <vector>

class DistressSignal {
  static void surround_with_brackets(std::list<char> &packet,
                                     std::list<char>::iterator content);
  static std::string is_right_order(std::list<char> &left,
                                    std::list<char> &right,
                                    std::list<char>::iterator &leftPacket,
                                    std::list<char>::iterator &rightPacket,
                                    int depth);

public:
  static int sum_of_right_pairs(std::vector<std::string> pairs);
  static std::vector<std::string>::size_type
  get_decoder_key(std::vector<std::string> pairs);
};

#endif