#ifndef DAY02_H
#define DAY02_H

std::vector<std::pair<char, char> > InputFileToPairs(std::string file_name);

int DecryptStrategyGuideIncorrectly(
    const std::vector<std::pair<char, char> >& encrypted_strategy_guide);

int DecryptStrategyGuide(
    const std::vector<std::pair<char, char> >& encrypted_strategy_guide);

#endif
