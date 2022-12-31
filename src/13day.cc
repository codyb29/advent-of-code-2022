#include "13day.h"
#include <algorithm>
#include <iostream>
#include <list>

int DistressSignal::sum_of_right_pairs(std::vector<std::string> pairs) {
  int sum = 0;
  for (std::vector<std::string>::size_type i = 0, j = 1, k = 1;
       i < pairs.size(); i += 3, j += 3, k++) {

    std::list<char> leftList;
    std::list<char> rightList;
    std::copy(pairs[i].begin(), pairs[i].end(), std::back_inserter(leftList));
    std::copy(pairs[j].begin(), pairs[j].end(), std::back_inserter(rightList));
    std::list<char>::iterator leftBegin = leftList.begin();
    std::list<char>::iterator rightBegin = rightList.begin();
    std::string ordering =
        is_right_order(leftList, rightList, leftBegin, rightBegin, 0);
    if (ordering == RIGHT_ORDER) {
      sum += k;
    }
  }

  return sum;
}

std::vector<std::string>::size_type
DistressSignal::get_decoder_key(std::vector<std::string> pairs) {
  std::vector<std::string> fixed_pairs;
  std::copy_if(pairs.begin(), pairs.end(), std::back_inserter(fixed_pairs),
               [](std::string packet) { return packet.length() > 0; });
  fixed_pairs.push_back("[2]");
  fixed_pairs.push_back("[6]");
  std::sort(fixed_pairs.begin(), fixed_pairs.end(),
            [](const std::string &packet1, const std::string &packet2) {
              std::list<char> leftList;
              std::list<char> rightList;
              std::copy(packet1.begin(), packet1.end(),
                        std::back_inserter(leftList));
              std::copy(packet2.begin(), packet2.end(),
                        std::back_inserter(rightList));
              std::list<char>::iterator leftBegin = leftList.begin();
              std::list<char>::iterator rightBegin = rightList.begin();
              std::string ordering =
                  is_right_order(leftList, rightList, leftBegin, rightBegin, 0);
              return ordering == RIGHT_ORDER;
            });
  std::vector<std::string>::iterator two_itr =
      std::find(fixed_pairs.begin(), fixed_pairs.end(), "[2]");
  std::vector<std::string>::iterator six_itr =
      std::find(fixed_pairs.begin(), fixed_pairs.end(), "[6]");
  std::vector<std::string>::size_type two =
      std::distance(fixed_pairs.begin(), two_itr) + 1;
  std::vector<std::string>::size_type six =
      std::distance(fixed_pairs.begin(), six_itr) + 1;

  return two * six;
}

void DistressSignal::surround_with_brackets(std::list<char> &packet,
                                            std::list<char>::iterator content) {
  packet.insert(content, '[');
  while ('0' <= *content && *content <= '9') {
    content++;
  }
  packet.insert(content, ']');
}

std::string
DistressSignal::is_right_order(std::list<char> &left, std::list<char> &right,
                               std::list<char>::iterator &leftPacket,
                               std::list<char>::iterator &rightPacket,
                               int depth) {
  if (*leftPacket == ']' || *rightPacket == ']') {
    return *rightPacket != ']' ? RIGHT_ORDER : WRONG_ORDER;
  }

  std::string order = NO_ORDER;
  // We need to continue to investigate so long as we are not at the end of this
  // layer's list.
  while (*leftPacket != ']' && *rightPacket != ']') {
    // we have two lists
    if (*leftPacket == '[' && *rightPacket == '[') {
      order =
          is_right_order(left, right, ++leftPacket, ++rightPacket, depth + 1);
      // will end on end bracket. move on to the next sequence. Guaranteed to
      // halt since this only gets reached when we're not in root level. There
      // will always be a root level ] to end our program.
      if (order == NO_ORDER) {
        if (*leftPacket == ']') {
          leftPacket++;
        }
        if (*rightPacket == ']') {
          rightPacket++;
        }
      }
      // we have mixed types
    } else if (*leftPacket == '[') {
      surround_with_brackets(right, rightPacket);
      rightPacket--;
      order = is_right_order(left, right, leftPacket, rightPacket, depth + 1);
      // we have mixed types
    } else if (*rightPacket == '[') {
      surround_with_brackets(left, leftPacket);
      leftPacket--;
      order = is_right_order(left, right, leftPacket, rightPacket, depth + 1);
      // we must have two integers.
    } else {
      // convert the string to number.
      std::string leftNumStr = "";
      std::string rightNumStr = "";

      while ('0' <= *leftPacket && *leftPacket <= '9') {
        leftNumStr += *leftPacket++;
      }
      while ('0' <= *rightPacket && *rightPacket <= '9') {
        rightNumStr += *rightPacket++;
      }

      int leftNum = std::stoi(leftNumStr);
      int rightNum = std::stoi(rightNumStr);

      if (leftNum < rightNum) {
        order = RIGHT_ORDER;
      } else if (leftNum > rightNum) {
        order = WRONG_ORDER;
      }
    }

    // Can we determine an order?
    if (order != NO_ORDER) {
      return order;
    }

    // ignore commas.
    if (*leftPacket == ',') {
      leftPacket++;
    }
    if (*rightPacket == ',') {
      rightPacket++;
    }
  }

  // if we aren't at root depth, we can't necessarily say they are not in order
  // just because r hit ]. There could be more items left in r in the upper
  // depths.
  if (depth > 1) {
    return order;
  }

  // if l hit ], and only l, they are in order, return out true. if r hit ]
  // in any capacity, they are not in order, return out false.
  return *rightPacket != ']' ? RIGHT_ORDER : WRONG_ORDER;
}
