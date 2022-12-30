#ifndef DAY12_H
#define DAY12_H

#include <string>
#include <unordered_set>
#include <vector>

struct Node {
  std::vector<std::string>::size_type i;
  std::string::size_type j;
  char letter;
  std::string sequence;

  Node(std::vector<std::string>::size_type i, std::string::size_type j,
       char letter, std::string sequence) {
    this->i = i;
    this->j = j;
    this->letter = letter;
    this->sequence = sequence + letter;
  }
};

class HillClimbingAlgorithm {
  const static int direction[][2];
  static std::string GetKey(int a, int b);

public:
  static std::vector<std::string>::size_type
  GetShortestPath(std::vector<std::string> elevation_map);
};

#endif
