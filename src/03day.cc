#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "03day.h"

std::vector<std::pair<std::string, std::string> >
RucksackOrganization::ConvertRucksackIntoPairs(
    const std::vector<std::string>& rucksacks) {
    std::vector<std::pair<std::string, std::string> > rucksack_pairs;
    for (std::vector<std::string>::const_iterator rucksack = rucksacks.begin();
         rucksack != rucksacks.end(); ++rucksack) {
        int half = rucksack->length() / 2;
        rucksack_pairs.push_back(
            std::make_pair(rucksack->substr(0, half), rucksack->substr(half)));
    }
    return rucksack_pairs;
}

std::vector<std::string> RucksackOrganization::ConvertInputIntoRucksacks(
    std::string filename) {
    std::ifstream input(filename);
    std::vector<std::string> rucksacks;
    std::string line;
    while (std::getline(input, line)) {
        rucksacks.push_back(line);
    }
    return rucksacks;
}

int RucksackOrganization::SumOfPriorities(
    const std::vector<std::pair<std::string, std::string> >& rucksacks) {
    int sum = 0;

    for (std::vector<std::pair<std::string, std::string> >::const_iterator itr =
             rucksacks.begin();
         itr != rucksacks.end(); ++itr) {
        std::unordered_set<char> items_in_first_sack;
        for (std::string::const_iterator sack1 = itr->first.begin();
             sack1 != itr->first.end(); ++sack1) {
            items_in_first_sack.insert(*sack1);
        }

        std::string::const_iterator sack2 = itr->second.begin();
        while (items_in_first_sack.find(*sack2) == items_in_first_sack.end() &&
               sack2 != itr->second.end()) {
            ++sack2;
        }

        if (sack2 != itr->second.end()) {
            sum += std::islower(*sack2) ? *sack2 - 'a' + 1 : *sack2 - 'A' + 27;
        }
    }

    return sum;
}

int RucksackOrganization::SumOfBadges(
    const std::vector<std::string>& rucksacks) {
    int sum = 0;
    std::unordered_map<char, int> items_count;

    for (std::vector<std::string>::size_type i = 0; i < rucksacks.size(); i++) {
        std::string::const_iterator item = rucksacks[i].begin();
        int curr = i % 3;
        while (item != rucksacks[i].end()) {
            if (items_count[*item] == curr) {
                items_count[*item]++;
                if (items_count[*item] == 3) break;
            }
            ++item;
        }
        if (((i + 1) % 3) == 0) {
            sum += std::islower(*item) ? *item - 'a' + 1 : *item - 'A' + 27;
            items_count.clear();
        }
    }

    return sum;
}

