#ifndef DAY08_H
#define DAY08_H

class TreetopTreeHouse {
    static int SearchAllDirections(
        const int i, const int j,
        const std::vector<std::vector<int>> &tree_heights);

   public:
    static std::vector<std::vector<int>> ConvertToHeights(
        std::vector<std::string> string_heights);
    static int CountTreesVisible(std::vector<std::vector<int>> tree_heights);
    static int HighestScenicScore(std::vector<std::vector<int>> tree_heights);
};

#endif

