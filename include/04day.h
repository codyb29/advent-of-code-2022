#ifndef DAY04_H
#define DAY04_H

class CleanCamp {
   public:
    static int CountOverlappingPairs(
        const std::vector<std::string>& assignments,
        bool (*is_overlapping)(int[2][2]));
    static bool IsFullyOverlapping(int intervals[2][2]);
    static bool IsPartiallyOverlapping(int intervals[2][2]);
};

#endif
