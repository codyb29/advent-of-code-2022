#ifndef DAY03_H
#define DAY03_H

class RucksackOrganization {
   public:
    static std::vector<std::pair<std::string, std::string> >
    ConvertRucksackIntoPairs(const std::vector<std::string>& rucksacks);

    static std::vector<std::string> ConvertInputIntoRucksacks(
        std::string filename);
    static int SumOfPriorities(
        const std::vector<std::pair<std::string, std::string> >& rucksacks);
    static int SumOfBadges(const std::vector<std::string>& rucksacks);
};
#endif
