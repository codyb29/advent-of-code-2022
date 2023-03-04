#include "25day.h"

#include <array>
#include <cmath>
#include <list>

long long decode(const std::string &snafu) {
  long long decimal = 0;
  long long square_5 = 1;
  for (std::string::const_reverse_iterator it = snafu.rbegin();
       it != snafu.rend(); ++it) {
    long long curr_val = 0;
    switch (*it) {
    case '=':
      curr_val = -2 * square_5;
      break;
    case '-':
      curr_val = -square_5;
      break;
    default:
      curr_val = (*it - '0') * square_5;
    }
    decimal += curr_val;
    square_5 *= 5;
  }
  return decimal;
}

std::string encode(const long long &decoded) {
  std::string snafu = "";
  std::list<long long> offsets;
  long long offset = 1;
  long long max_so_far = 0;
  while (((offset * 2) + max_so_far) <= decoded) {
    offsets.emplace_front(offset);
    max_so_far += (2 * offset);
    offset *= 5;
  }

  // handle msb
  long long curr = offset;
  if (decoded <= (offset + max_so_far)) {
    snafu.append("1");
  } else {
    snafu.append("2");
    curr = offset * 2;
  }

  for (std::list<long long>::const_iterator it = offsets.begin();
       it != offsets.end(); ++it) {
    long long base = decoded - curr;
    std::array<long long, 5>::size_type smallest_diff = 0;
    std::array<long long, 5> possible_vals = {*it * -2, -*it, 0, *it, *it * 2};
    for (std::array<long long, 5>::size_type i = 1; i < possible_vals.size();
         i++) {
      if (std::abs(base - possible_vals[i]) <
          std::abs(base - possible_vals[smallest_diff])) {
        smallest_diff = i;
      }
    }
    switch (smallest_diff) {
    case 0:
      snafu.append("=");
      break;
    case 1:
      snafu.append("-");
      break;
    case 3:
      snafu.append("1");
      break;
    case 4:
      snafu.append("2");
      break;
    case 2:
    default:
      snafu.append("0");
    }
    curr += possible_vals[smallest_diff];
  }

  return snafu;
}

std::string full_of_hot_air_pt1(const std::vector<std::string> &puzzle_input) {
  long long sum = 0;
  for (std::vector<std::string>::const_iterator it = puzzle_input.begin();
       it != puzzle_input.end(); ++it) {
    sum += decode(*it);
  }
  return encode(sum);
}