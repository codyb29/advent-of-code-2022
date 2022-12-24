#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#define WON 6
#define TIE 3

std::vector<std::pair<char, char> > InputFileToPairs(std::string file_name) {
  std::vector<std::pair<char, char> > pairs;
  std::ifstream input(file_name);

  std::string line;
  while (std::getline(input, line)) {
    pairs.push_back(std::make_pair(line[0], line[2]));
  }

  return pairs;
}

int DecryptStrategyGuideIncorrectly(
    const std::vector<std::pair<char, char> >& encrypted_strategy_guide) {
  int score = 0;

  for (std::vector<std::pair<char, char> >::const_iterator game =
           encrypted_strategy_guide.begin();
       game != encrypted_strategy_guide.end(); ++game) {
    if (game->second == 'X') {
      score += 1;
      if (game->first == 'C') {
        score += WON;
      }
    } else if (game->second == 'Y') {
      score += 2;
      if (game->first == 'A') {
        score += WON;
      }
    } else {
      score += 3;
      if (game->first == 'B') {
        score += WON;
      }
    }

    if (game->first - 'A' == game->second - 'X') {
      score += TIE;
    }
  }
  return score;
}

int DecryptStrategyGuide(
    const std::vector<std::pair<char, char> >& encrypted_strategy_guide) {
  int score = 0;

  for (std::vector<std::pair<char, char> >::const_iterator game =
           encrypted_strategy_guide.begin();
       game != encrypted_strategy_guide.end(); ++game) {
    if (game->first == 'A') {
      if (game->second == 'X') {
        score += 3;
      } else if (game->second == 'Y') {
        score += 1 + TIE;
      } else {
        score += 2 + WON;
      }
    } else if (game->first == 'B') {
      if (game->second == 'X') {
        score += 1;
      } else if (game->second == 'Y') {
        score += 2 + TIE;
      } else {
        score += 3 + WON;
      }
    } else {
      if (game->second == 'X') {
        score += 2;
      } else if (game->second == 'Y') {
        score += 3 + TIE;
      } else {
        score += 1 + WON;
      }
    }
  }
  return score;
}

