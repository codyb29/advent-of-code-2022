#include <vector>
#include <string>

#include "04day.h"


int CleanCamp::CountOverlappingPairs(const std::vector<std::string>& assignments,
                          bool (*is_overlapping)(int[2][2])) {
    int count = 0;
    for (std::vector<std::string>::const_iterator assignment =
             assignments.begin();
         assignment != assignments.end(); ++assignment) {
        std::string::size_type comma = assignment->find(',');
        std::string section1 = assignment->substr(0, comma);
        std::string section2 = assignment->substr(comma + 1);
        std::string::size_type first_dash = section1.find('-');
        std::string::size_type second_dash = section2.find('-');
        int section1_start = std::stoi(section1.substr(0, first_dash));
        int section1_end = std::stoi(section1.substr(first_dash + 1));
        int section2_start = std::stoi(section2.substr(0, second_dash));
        int section2_end = std::stoi(section2.substr(second_dash + 1));
        int intervals[2][2] = {{section1_start, section1_end},
                               {section2_start, section2_end}};
        if ((*is_overlapping)(intervals)) {
            count++;
        }
    }
    return count;
}

bool CleanCamp::IsFullyOverlapping(int intervals[2][2]) {
    return (intervals[0][0] <= intervals[1][0] &&
            intervals[0][1] >= intervals[1][1]) ||
           (intervals[1][0] <= intervals[0][0] &&
            intervals[1][1] >= intervals[0][1]);
}

bool CleanCamp::IsPartiallyOverlapping(int intervals[2][2]) {
    return (intervals[0][0] <= intervals[1][0] &&
            intervals[0][1] >= intervals[1][0]) ||
           (intervals[1][0] <= intervals[0][0] &&
            intervals[1][1] >= intervals[0][0]);
}

